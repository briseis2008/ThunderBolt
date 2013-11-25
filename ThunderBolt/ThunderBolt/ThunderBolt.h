

#ifndef __Thunder__ThunderBolt__
#define __Thunder__ThunderBolt__

#include <stdio.h>
#include <math.h>
#include "texture_loader.h"
#include "vector.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "color.h"
#include "missile.h"
#include "Plane.h"
#include "debug.h"
#include "enemy.h"
#include "player.h"
#include "linked_list.h"
#include "boss.h"
#include "prize.h"


#define GAME_START    0
#define GAME_RUNNING  1
#define GAME_END      2
#define GAME_PAUSE    3

#define MANUE_OPTION_CONTINUE 0
#define MANUE_OPTION_START    1
#define MANUE_OPTION_QUIT     2
#define MANUE_OPTION_MAX      3

#define ENEMY_NUM_MAX  10



class ThunderBolt {
    int state;
    int terminate;
    int game_option;
    
    PlaneList enemyList;
    PlaneList playerList;
    PlaneList prizeList;
    MissileList enemyMissleList;
    MissileList playerMissleList;
    
public:
    void resetGame();
    void gameStart(int key);
    void gamePause(int key);
    void gameRun(int key);
    void generateEnemy();
    void generatePrize();
    void generatePlayer();
    void draw();
    void drawBackground();
    void drawStartMenu();
    void drawPauseMenu();
    void drawPlanes();
    void drawPrizes();
    void drawMissiles();
    
public:
    void setup();
    void run();
    void cleanUp();
};

#endif /* defined(__Thunder__ThunderBolt__) */
