

#ifndef __Thunder__ThunderBolt__
#define __Thunder__ThunderBolt__

#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "color.h"
#include "missile.h"
#include "Plane.h"


#define GAME_START   0
#define GAME_RUNNING 1
#define GAME_END     2

#define MANUE_OPTION_START 1
#define MANUE_OPTION_QUIT  2

class ThunderBolt {
    int life;
    int state;
    int terminate;
    int game_option;
    
    PlaneList enemyList;
    PlaneList playerList;
    MissileList enemyMissleList;
    MissileList playerMissleList;
    
public:
    void resetGame();
    void draw();
    void drawBackground();
    void drawMenu();
    void drawPlanes();
    void drawPrizes();
    void drawMissiles();
    
public:
    void setup();
    void run();
};

#endif /* defined(__Thunder__ThunderBolt__) */
