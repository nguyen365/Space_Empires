#include "Object.h"
#include <boost/lexical_cast.hpp>
#include <cmath>

//debug
#include <iostream>

const unsigned CScreenWidth = 160;
const unsigned CScreenHeight = 100;
const unsigned CMapViewWidth = 100;
const unsigned CMapViewHeight = 100;
const unsigned CInfoCentre = (CScreenWidth-CMapViewWidth)/2;
const char* CWindowName= "Space Empires";
const unsigned CFPS = 120;
const int CMoveHorizontal = 1;
const int CMoveVertical = 1;
enum handleKeysCode {DO_NOTHING,DO_CLOSE_WINDOW,
                     MOVE_SCREEN_RIGHT,MOVE_SCREEN_LEFT,
                     MOVE_SCREEN_UP, MOVE_SCREEN_DOWN};
enum SCREEN_STATES {SYSTEM_VIEW};
enum ScreenConsolePairs  {GalaxyViewPair,
                          InfoBarPair};
enum InfoBarItems {eNumPlanetPrefix,
                   eNumPlanet};
const ScreenString CNumPlanetString = ScreenString("Number of Planets:",
                                                   TCOD_CENTER,
                                                   CInfoCentre,
                                                   10);

ScreenObject::ScreenObject()
{
    Coordinates[0] = 0;
    Coordinates[1] = 0;
    ObjectChar = 0;
}

ScreenObject::ScreenObject(System iSystem)
{
    Coordinates[0] = floor(iSystem.getXCoordinate() +0.5);
    Coordinates[1] = floor(iSystem.getYCoordinate() +0.5);
    ObjectChar = 'O';
}

ScreenObject::ScreenObject(int iX, int iY, int iChar)
{
    Coordinates[0] = iX;
    Coordinates[1] = iY;
    ObjectChar = iChar;
}

ScreenObject::~ScreenObject()
{

}

int ScreenObject::getX()
{
    return Coordinates[0];
}

int ScreenObject::getY()
{
    return Coordinates[1];
}

bool ScreenObject::moveBy(int iX, int iY)
{
    Coordinates[0] += iX;
    Coordinates[1] += iY;
    return true;
}

bool ScreenObject::moveTo(int iX, int iY)
{
    Coordinates[0] = iX;
    Coordinates[1] = iY;
    return true;
}

void ScreenObject::draw(TCODConsole* iConsole)
{
    iConsole->putChar(Coordinates[0],Coordinates[1],ObjectChar);
}

void ScreenObject::clear(TCODConsole* iConsole)
{
    iConsole->putChar(Coordinates[0],Coordinates[1],' ');
}

ScreenString::ScreenString(std::string iString, TCOD_alignment_t iAlignment, int iX, int iY)
: ScreenObject(iX,iY,0)
{
    cString = iString;
    cAlignment = iAlignment;
}

TCOD_alignment_t ScreenString::getAlignment()
{
    return cAlignment;
}

void ScreenString::draw(TCODConsole* iConsole)
{
    if( iConsole->getAlignment() != cAlignment)
        iConsole->setAlignment(cAlignment);
    iConsole->print(this->getX(),this->getY(),cString.c_str());
}

void ScreenString::clear(TCODConsole* iConsole)
{
    if (cAlignment == TCOD_LEFT)
        iConsole->rect(this->getX(), this->getY(),
                      cString.size(),1,true);
    else if (cAlignment == TCOD_CENTER)
        iConsole->rect(this->getX()-((cString.size()+1)/2), this->getY(),
                      cString.size(),1,true);
    else if (cAlignment == TCOD_RIGHT)
        iConsole->rect(this->getX()-cString.size(), this->getY(),
                      cString.size(),1,true);
}

bool ScreenString::newString(std::string iString)
{
    cString = iString;
    return true;
}

Screen::Screen()
: CurrentState(SYSTEM_VIEW)
{
    CurrentViewTopLeft[0] = 0;
    CurrentViewTopLeft[1] = 0;
    CurrentViewBottomRight[0] = 0;
    CurrentViewBottomRight[1] = 0;
    cCursor = ScreenObject(-1,-1,0);
}

Screen::Screen(unsigned iState, unsigned iTopX, unsigned iTopY,
               unsigned iBottomX, unsigned iBottomY)
: CurrentState(iState)
{
    CurrentViewTopLeft[0] = iTopX;
    CurrentViewTopLeft[1] = iTopY;
    CurrentViewBottomRight[0] = iBottomX;
    CurrentViewBottomRight[1] = iBottomY;
    cCursor = ScreenObject(-1,-1,0);
}

Screen::~Screen()
{

}

bool Screen::addObject(ScreenObject iObject)
{
    ObjectList.push_back(iObject);
    return true;
}

bool Screen::addString(ScreenString iString)
{
    StringList.push_back(iString);
    return true;
}

bool Screen::drawObjects(TCODConsole* iConsole)
{
    for (unsigned i = 0; i < ObjectList.size() ; i++)
    {
        //if (ObjectList[i].getX() < CurrentViewBottomRight[0] &&
            //ObjectList[i].getY() < CurrentViewBottomRight[1])
            ObjectList[i].draw(iConsole);
    }
    for (unsigned i = 0; i < StringList.size(); i++)
    {
        StringList[i].draw(iConsole);
    }
    iConsole->setCharBackground(cCursor.getX(), cCursor.getY(), TCODColor::white);
    iConsole->setCharForeground(cCursor.getX(), cCursor.getY(), TCODColor::black);
    return true;
}

bool Screen::clear(TCODConsole* iConsole)
{
    iConsole->setCharBackground(cCursor.getX(), cCursor.getY(), TCODColor::black);
    iConsole->setCharForeground(cCursor.getX(), cCursor.getY(), TCODColor::white);
    for (unsigned i = 0; i < ObjectList.size() ; i++)
    {
        //if (ObjectList[i].getX() >= 0 && ObjectList[i].getY() >= 0)
          //  if (ObjectList[i].getX() < CurrentViewBottomRight[0] &&
            //    ObjectList[i].getY() < CurrentViewBottomRight[1])
                ObjectList[i].clear(iConsole);
    }
    for (unsigned i = 0; i < StringList.size(); i++)
    {
        StringList[i].clear(iConsole);
    }
    return true;
}

bool Screen::moveScreenBy(int iX, int iY)
{
    for(unsigned i = 0; i < ObjectList.size(); i++)
    {
        ObjectList[i].moveBy(-1 * iX,-1 *iY);
    }
    CurrentViewTopLeft[0] += iX;
    CurrentViewTopLeft[1] += iY;
    CurrentViewBottomRight[0] += iX;
    CurrentViewBottomRight[1] += iY;
    return true;
}

bool Screen::moveCursorTo(int iX, int iY)
{
    cCursor.moveTo(iX, iY);
    return true;
}

bool Screen::moveCursorBy(int iX, int iY)
{
    cCursor.moveBy(iX, iY);
    return true;
}

bool Screen::changeString(unsigned stringPos, std::string iString)
{
    return StringList[stringPos].newString(iString);
}

std::vector<unsigned> Screen::getCursorLocation()
{
    std::vector<unsigned> retVal;
    retVal.push_back(cCursor.getX());
    retVal.push_back(cCursor.getY());
    return  retVal;
}

std::vector<unsigned> Screen::getCurrentViewWindow()
{
    std::vector<unsigned> retVal;
    retVal.push_back(CurrentViewTopLeft[0]);
    retVal.push_back(CurrentViewTopLeft[1]);
    retVal.push_back(CurrentViewBottomRight[0]);
    retVal.push_back(CurrentViewBottomRight[1]);
    return  retVal;
}

GameApp::GameApp()
{
    TCODConsole::initRoot(CScreenWidth,CScreenHeight,CWindowName,false);
    cScreenPairList.push_back(ScreenPair(Screen(),new TCODConsole(CMapViewWidth,CMapViewHeight)));
    cScreenPairList.push_back(ScreenPair(Screen(),new TCODConsole(CScreenWidth-CMapViewWidth,CScreenHeight)));
    cGameState = SYSTEM_VIEW;
    iFrame = 0;
}

GameApp::~GameApp()
{

}

bool GameApp::init()
{
    //Import Galaxy Data
    cGalaxy = new Galaxy();
    std::vector<System> tempVector = cGalaxy->getSystemList();
    for (unsigned i = 0; i < tempVector.size() ; i++)
        cScreenPairList[GalaxyViewPair].first.addObject(ScreenObject(tempVector[i]));
    //Create borders around map's edge
    for (unsigned i = 0; i < CGridSpace[0]; i++)
    {
        cScreenPairList[GalaxyViewPair].first.addObject(ScreenObject(-1,i,'X'));
        cScreenPairList[GalaxyViewPair].first.addObject(ScreenObject(CGridSpace[0],i,'X'));
    }
    for (unsigned i = 0; i < CGridSpace[1]; i++)
    {
        cScreenPairList[GalaxyViewPair].first.addObject(ScreenObject(i,-1,'X'));
        cScreenPairList[GalaxyViewPair].first.addObject(ScreenObject(i,CGridSpace[1],'X'));
    }
    //Create Cursor
        cScreenPairList[GalaxyViewPair].first.moveCursorTo(CMapViewWidth/2,CMapViewHeight/2);
    //Create Info Bar
    for (unsigned i = 0; i < CScreenHeight; i++)
        cScreenPairList[InfoBarPair].first.addObject(ScreenObject(0,i,'+'));
    cScreenPairList[InfoBarPair].first.addString
        (CNumPlanetString);
    cScreenPairList[InfoBarPair].first.addString
        (ScreenString("0",TCOD_CENTER,CInfoCentre,11));
    return true;
}

void GameApp::run()
{
    while ( !TCODConsole::isWindowClosed() )
    {
        //TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,NULL,NULL);
        if (handleKeys(TCODConsole::checkForKeypress(TCOD_KEY_PRESSED)) == DO_CLOSE_WINDOW)
            return;
        updateInfoBar();
        for (unsigned i = 0; i < cScreenPairList.size(); i++)
            cScreenPairList[i].first.drawObjects(cScreenPairList[i].second);
        TCODConsole::blit(cScreenPairList[GalaxyViewPair].second, 0, 0, 0, 0,
                          TCODConsole::root, 0, 0);
        TCODConsole::blit(cScreenPairList[InfoBarPair].second, 0, 0, 0, 0,
                          TCODConsole::root, CMapViewWidth, 0);
        TCODConsole::flush();
        for (unsigned i = 0; i < cScreenPairList.size(); i++)
            cScreenPairList[i].first.clear(cScreenPairList[i].second);
    }
}

int GameApp::handleKeys(TCOD_key_t iKey)
{
    if (cGameState == SYSTEM_VIEW)
    {
        if (iKey.vk == TCODK_ESCAPE)
            return DO_CLOSE_WINDOW;
        else if (iKey.vk == TCODK_RIGHT)
            cScreenPairList[GalaxyViewPair].first.moveScreenBy(CMoveHorizontal,0);
        else if (iKey.vk == TCODK_LEFT)
            cScreenPairList[GalaxyViewPair].first.moveScreenBy(-1 * CMoveHorizontal,0);
        else if (iKey.vk == TCODK_UP)
            cScreenPairList[GalaxyViewPair].first.moveScreenBy(0,-1 * CMoveVertical);
        else if (iKey.vk == TCODK_DOWN)
            cScreenPairList[GalaxyViewPair].first.moveScreenBy(0,CMoveVertical);
        else if (iKey.vk == TCODK_KP1)
            cScreenPairList[GalaxyViewPair].first.moveCursorBy(-1,1);
        else if (iKey.vk == TCODK_KP2)
            cScreenPairList[GalaxyViewPair].first.moveCursorBy(0,1);
        else if (iKey.vk == TCODK_KP3)
            cScreenPairList[GalaxyViewPair].first.moveCursorBy(1,1);
        else if (iKey.vk == TCODK_KP4)
            cScreenPairList[GalaxyViewPair].first.moveCursorBy(-1,0);
        else if (iKey.vk == TCODK_KP6)
            cScreenPairList[GalaxyViewPair].first.moveCursorBy(1,0);
        else if (iKey.vk == TCODK_KP7)
            cScreenPairList[GalaxyViewPair].first.moveCursorBy(-1,-1);
        else if (iKey.vk == TCODK_KP8)
            cScreenPairList[GalaxyViewPair].first.moveCursorBy(0,-1);
        else if (iKey.vk == TCODK_KP9)
            cScreenPairList[GalaxyViewPair].first.moveCursorBy(1,-1);
    }
    return DO_NOTHING;
}

void GameApp::updateInfoBar()
{
    std::vector<unsigned> tempGalaxyCursorLocation =
        cScreenPairList[GalaxyViewPair].first.getCursorLocation();
    std::vector<unsigned> tempGalaxyView =
        cScreenPairList[GalaxyViewPair].first.getCurrentViewWindow();
    unsigned numPlanetHighlighted = 0;
    const System* tempSystemPtr =
    (cGalaxy->findSystem(tempGalaxyView[0] + tempGalaxyCursorLocation[0],
        tempGalaxyView[1] + tempGalaxyCursorLocation[1]));
    if (tempSystemPtr)
        numPlanetHighlighted = tempSystemPtr->getNumPlanets();
    cScreenPairList[InfoBarPair].first.changeString
        (eNumPlanet, boost::lexical_cast<std::string>(numPlanetHighlighted));
}

Object::Object(double iMass, threeDVector inputPosition, threeDVector inputVel)
: cMass(iMass)
{
    posVector = inputPosition;
    velVector = inputVel;
}

Object::~Object()
{
    //dtor
}

void Object::iterate(double timeElapsed)
{
    threeDVector accerlationVector;
    for (unsigned i = 0; i < forceList.size(); i++)
    {
        if (forceList.at(i).second.first)
        {
                double tempDouble = (forceList.at(i).second.second > timeElapsed ?
                                     timeElapsed : forceList.at(i).second.second);
                forceList.at(i).second.second = forceList.at(i).second.second - timeElapsed;
                accerlationVector = addVector(accerlationVector,mulVector(tempDouble,forceList.at(i).first));
        }
        else
        {

        }
    }
}

threeDVector addVector(threeDVector A, threeDVector B)
{
    threeDVector retVal;
    for (unsigned i = 0; i < 3; i++)
        retVal.push_back(A[i]+B[i]);
    return retVal;
}

threeDVector mulVector(double K, threeDVector A)
{
    threeDVector retVal;
    for (unsigned i = 0; i < 3; i++)
        retVal.push_back(A[i] * K);
    return retVal;
}

double getTime(double A, double B)
{
    return A - B >= 0 ? A - B : 0;
}
