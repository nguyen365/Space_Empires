#include "libtcod.hpp"
#include <string>
#include <utility>
#include <vector>
#include "System.h"

typedef std::vector<double> threeDVector;
typedef std::pair <threeDVector,std::pair <bool,double> > force;


const double DEFAULT_ITERATION_TIME = 1;
const std::vector<double> ZERO_VECTOR(3,0);

#ifndef __OBJECT_H__
#define __OBJECT_H__

extern const unsigned CScreenWidth;
extern const unsigned CScreenHeight;
extern const unsigned CMapViewWidth;
extern const unsigned CMapViewHeight;
extern const char* CWindowName;
extern const unsigned CFPS;
extern const int CMoveHorizontal;
extern const int CMoveVertical;
class Screen;
typedef std::pair <Screen,TCODConsole*> ScreenPair;

threeDVector addVector(threeDVector A, threeDVector B);
threeDVector mulVector(double K, threeDVector A);
double getTime(double A, double B);

class ScreenObject
{
    private:
        int Coordinates [2];
        int ObjectChar;
    public:
        ScreenObject();
        ScreenObject(System);
        ScreenObject(int, int, int);
        ~ScreenObject();
        int getX();
        int getY();
        virtual bool moveBy(int, int);
        virtual bool moveTo(int, int);
        virtual void draw(TCODConsole* iConsole);
        virtual void clear(TCODConsole* iConsole);
};

class ScreenString : public ScreenObject
{
    private:
        std::string cString;
        TCOD_alignment_t cAlignment;
    public:
        ScreenString(std::string, TCOD_alignment_t, int, int);
        TCOD_alignment_t getAlignment();
        void draw(TCODConsole* iConsole);
        void clear(TCODConsole* iConsole);
        bool newString(std::string iString);
};

class Screen
{
    private:
        unsigned CurrentState;
        unsigned CurrentViewTopLeft [2];
        unsigned CurrentViewBottomRight [2];
        ScreenObject cCursor;
        std::vector<ScreenObject> ObjectList;
        std::vector<ScreenString> StringList;
    public:
        Screen();
        Screen(unsigned,unsigned,unsigned,unsigned,unsigned);
        ~Screen();
        bool addObject(ScreenObject iObject);
        bool addString(ScreenString iString);
        bool drawObjects(TCODConsole* iConsole);
        bool clear(TCODConsole* iConsole);
        bool moveScreenBy(int, int);
        bool moveCursorTo(int, int);
        bool moveCursorBy(int, int);
        bool changeString(unsigned, std::string);
        std::vector<unsigned> getCursorLocation();
        std::vector<unsigned> getCurrentViewWindow();
    protected:
    private:
};

class GameApp
{
    private:
    Galaxy* cGalaxy;
    std::vector<ScreenPair> cScreenPairList;
    unsigned cGameState;
    unsigned iFrame;
    public:
        GameApp();
        ~GameApp();
        bool init();
        void run();
    protected:
    private:
        int handleKeys(TCOD_key_t iKey);
        void updateInfoBar();
};

class Object
{
    private:
        double cMass;
        threeDVector posVector;
        threeDVector velVector;
        std::vector<force> forceList;
    public:
        Object(double iMass = 0, threeDVector inputPosition = ZERO_VECTOR, threeDVector inputVel = ZERO_VECTOR);
        virtual ~Object();
        void iterate(double timeElapsed = DEFAULT_ITERATION_TIME);
        bool addForce(force newForce);
        bool removeForce(unsigned index);
        bool removeForce(force* forcePTR);
        bool applyInstanteousForce(force Force);
        bool applyInstanteousForce(threeDVector Force);
    protected:
    private:
};

#endif // __OBJECT_H__
