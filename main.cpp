#include "libtcod.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include "Object.h"

int main() {
    GameApp iGame;
    iGame.init();
    iGame.run();

    return 0;
}

