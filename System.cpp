#include "System.h"

boost::random::mt19937 CRandomGen;

int rollDie(int min, int max)
{
    boost::random::uniform_int_distribution<> dist(min, max);
    return dist(CRandomGen);
}

double average(double a, double b)
{
    return int((a + b)/2);
}

Planet::Planet()
{

}

Planet::Planet(unsigned iLevel)
: cLevel(iLevel)
{

}

Planet::~Planet()
{

}

System::System()
{
    //ctor
}

System::System(double iXCoor, double iYCoor)
{
    cGalaxyCoordinates[0] = iXCoor;
    cGalaxyCoordinates[1] = iYCoor;
    unsigned numPlanets = rollDie(CNumPlanets[0],CNumPlanets[1]);
    for (unsigned i = 0; i < numPlanets; i++)
        addRandomPlanet(int(CPlanetLevel[0]),int(CPlanetLevel[1]));
}

System::~System()
{
    //dtor
}

//void System::Draw(TCODConsole* iConsole)
//{
//    iConsole->putChar(cGalaxyCoordinates[0],cGalaxyCoordinates[1],'O');
//}
//
//void System::Clear(TCODConsole* iConsole)
//{
//    iConsole->putChar(cGalaxyCoordinates[0],cGalaxyCoordinates[1],' ');
//}

bool System::setCoordinatesTo(double iX, double iY)
{
    cGalaxyCoordinates[0] = iX;
    cGalaxyCoordinates[1] = iY;
    return true;
}

bool System::addPlanet(unsigned iLevel = CDefaultPlanetLevel)
{
    cPlanetList.push_back(Planet(iLevel));
    return true;
}

bool System::addRandomPlanet(unsigned iLower, unsigned iUpper)
{
    return addPlanet(rollDie(iLower,iUpper));
}

unsigned System::getNumPlanets() const
{
    return cPlanetList.size();
}

double System::findDistanceTo(System* iSystem)
{
    return sqrt(pow(this->cGalaxyCoordinates[0] - iSystem->cGalaxyCoordinates[0],2) +
                pow(this->cGalaxyCoordinates[1] - iSystem->cGalaxyCoordinates[1],2));
}

double System::getXCoordinate()
{
    return cGalaxyCoordinates[0];
}

double System::getYCoordinate()
{
    return cGalaxyCoordinates[1];
}

Galaxy::Galaxy()
{
    unsigned tempNumStars = rollDie(CDebugNumStars[0],CDebugNumStars[1]);
    for (unsigned i = 0; i < tempNumStars; i++)
        cSystemList.push_back(System(rollDie(0,CGridSpace[0]),rollDie(0,CGridSpace[1])));
    for (unsigned i = 0; i < numClusteriseStars; i++)
        ClusteriseStars();
}

Galaxy::~Galaxy()
{

}

//void Galaxy::Draw(TCODConsole* iConsole)
//{
//    for (unsigned i = 0; i < cSystemList.size(); i++)
//        cSystemList[i].Draw(iConsole);
//}
//
//void Galaxy::Clear(TCODConsole* iConsole)
//{
//    for (unsigned i = 0; i < cSystemList.size(); i++)
//        cSystemList[i].Clear(iConsole);
//}

const std::vector<System> Galaxy::getSystemList()
{
    return cSystemList;
}

const System* Galaxy::findSystem(unsigned SystemX, unsigned SystemY)
{
    for (unsigned i = 0; i < cSystemList.size(); i++)
    {
        if (cSystemList[i].getXCoordinate() == SystemX &&
            cSystemList[i].getYCoordinate() == SystemY)
            {
                System *foundSystem = &(cSystemList[i]);
                return foundSystem;
            }
    }
    return NULL;
}

void Galaxy::ClusteriseStars()
{
    return;
    for (unsigned i = 0; i < cSystemList.size(); i++)
    {
        double tempClosest = DBL_MAX;
        unsigned closestSystem = 0;
        for (unsigned j = 0; j < cSystemList.size(); j++)
        {
            if (j == i)
                continue;
            if (cSystemList[i].findDistanceTo(&cSystemList[j])<tempClosest)
            {
                tempClosest = cSystemList[i].findDistanceTo(&cSystemList[j]);
                closestSystem = j;
            }
        }
        cSystemList[i].setCoordinatesTo(average(cSystemList[i].getXCoordinate(),cSystemList[closestSystem].getXCoordinate()),
                                        average(cSystemList[i].getYCoordinate(),cSystemList[closestSystem].getYCoordinate()));
    }
}

