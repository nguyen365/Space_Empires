//#include "Object.h"
#include "libtcod.hpp"
#include <cfloat>
#include <vector>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>


#ifndef __SYSTEM_H__
#define __SYSTEM_H__

const unsigned numClusteriseStars = 1;
const unsigned CDefaultPlanetLevel = 1;
const unsigned CGridSpace [] = {200,200};
const unsigned CDebugNumStars [] = {1,100};
const unsigned CNumPlanets [] = {0, 12};
const double CPlanetLevel [] = {1, 16};

int rollDie(int min, int max);
double average(double a, double b);

class StellarObject
{
    private:
    public:
        StellarObject();
        ~StellarObject();
};

class Star
{
    private:
    public:
        Star();
        ~Star();
    protected:
    private:
};

class Planet
{
    private:
        unsigned cLevel;
    public:
        Planet();
        Planet(unsigned);
        ~Planet();
    protected:
    private:
};

class System
{
    private:
        double cGalaxyCoordinates [2];
        std::vector<Planet> cPlanetList;
    public:
        System();
        System(double, double);
        virtual ~System();
//        void Draw(TCODConsole* iConsole);
//        void Clear(TCODConsole* iConsole);
        bool addPlanet(unsigned iLevel);
        bool addRandomPlanet(unsigned iLower, unsigned iUpper);
        bool setCoordinatesTo(double,double);
        unsigned getNumPlanets() const;
        double findDistanceTo(System* iSystem);
        double getXCoordinate();
        double getYCoordinate();
    protected:
    private:
};

class Galaxy
{
    private:
        std::vector<System> cSystemList;
    public:
        Galaxy();
        ~Galaxy();
        //void Draw(TCODConsole* iConsole);
        //void Clear(TCODConsole* iConsole);
        const std::vector<System> getSystemList();
        const System* findSystem(unsigned SystemX, unsigned SystemY);
    protected:
    private:
        void ClusteriseStars();
};

#endif // __SYSTEM_H__
