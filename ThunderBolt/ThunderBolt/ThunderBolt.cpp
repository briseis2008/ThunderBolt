#include "ThunderBolt.h"


void ThunderBolt::drawBackground() {
    /* draw background */
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 0);
    glVertex2d(0, 0);
    glVertex2d(WINDOW_WID, 0);
    glColor3ub(90, 90, 90);
    glVertex2d(WINDOW_WID, WINDOW_HEI);
    glColor3ub(50, 50, 50);
    glVertex2d(0, WINDOW_HEI);
    glEnd();
}


void ThunderBolt::drawStartMenu() {
    glBegin(GL_QUADS);
    if (game_option == MENU_OPTION_START) {
        glColor3ub(255, 255, 255);
    }
    else {
        glColor3ub(100,100,100);
    }
    glVertex2d(200, 400);
    glVertex2d(400, 400);
    glVertex2d(400, 440);
    glVertex2d(200, 440);
    glEnd();
    
    /* Game option: start */
    glColor3ub(0,0,0);
    glRasterPos2d(200, 440);
    YsGlDrawFontBitmap16x24("Start new game\n");
    glEnd();
    
    
    glBegin(GL_QUADS);
    if (game_option == MENU_OPTION_QUIT) {
        glColor3ub(255, 255, 255);
    }
    else {
        glColor3ub(100,100,100);
    }
    glVertex2d(200, 500);
    glVertex2d(400, 500);
    glVertex2d(400, 540);
    glVertex2d(200, 540);
    glEnd();
    
    /* Game option: start */
    glColor3ub(0,0,0);
    glRasterPos2d(200, 540);
    YsGlDrawFontBitmap16x24("QUIT\n");
    glEnd();
}

void ThunderBolt::drawPauseMenu() {
    this->drawBackground();
    glBegin(GL_QUADS);
    if (game_option == MENU_OPTION_CONTINUE) {
        glColor3ub(255, 255, 255);
    }
    else {
        glColor3ub(100,100,100);
    }
    glVertex2d(200, 300);
    glVertex2d(400, 300);
    glVertex2d(400, 340);
    glVertex2d(200, 340);
    glEnd();
    
    /* Game option: continue */
    glColor3ub(0,0,0);
    glRasterPos2d(200, 340);
    YsGlDrawFontBitmap16x24("Continue\n");
    glEnd();
    
    drawStartMenu();
}


void ThunderBolt::resetGame() {
    cleanUp();
    glClearColor(0, 0, 0, 1);
    glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    this->state = GAME_START;
    this->terminate = 0;
    this->game_option = MENU_OPTION_START;
    generateEnemy();
    generatePrize();
    generatePlayer();
}

void ThunderBolt::draw() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    /* Draw background */
    this->drawBackground();
    
    /* Draw Menu */
    if (state == GAME_START) {
        this->drawStartMenu();
    }
    else if (state == GAME_RUNNING){
        Plane *player = (playerList.getFront())->dat;
        player->Draw();
        
        /* traverse enemies and missiles, draw them*/
        
        PlaneNode *pNode;
        pNode = enemyList.getFront();
        FOR_EACH(pNode, enemyList) {
            pNode->dat->Draw();
        }
        
        MissileNode *node;
        node = playerMissleList.getFront();
        FOR_EACH(node, playerMissleList) {
            node->dat->Draw();
        }

        pNode = prizeList.getFront();
        FOR_EACH(pNode, prizeList) {
            pNode->dat->Draw();
        }
    }
    
    if (state == GAME_PAUSE) {
        this->drawPauseMenu();
    }

}

void ThunderBolt::setup() {
    FsOpenWindow(16, 16, WINDOW_WID, WINDOW_HEI, 1);
    textureInit();
    resetGame();
}

void ThunderBolt::generateEnemy() {
    int num = 3;
    for (int i = 0; i < num; i++) {
        int posX = rand() % (int)WINDOW_WID;
        int posY = 0;
        int dirX = rand() % 10;
        int dirY = rand() % 10;
        Plane *newP = new Enemy1(Vector2(posX, posY), Vector2(dirX, dirY));
        newP->setVelocity(2.0);
        enemyList.InsertBack(newP);
    }
    
    /* testing boss */
    Plane *boss = new Boss(Vector2(300, 100), Vector2(0,1));
    enemyList.InsertBack(boss);
}

void ThunderBolt::generatePlayer() {
    Vector2 startPosition(300, 280);
    Vector2 startDirection(0, -1);
    Plane *player = new Thunder(startPosition, startDirection);
    playerList.InsertBack(player);
    ((playerList.getFront())->dat)->setVelocity(5);
    /* Set player's initial weapon as bullet */
    ((Thunder *)(playerList.getFront())->dat)->SwitchWeapon(BULLET, playerMissleList);
    
}


//TODO
void ThunderBolt::generatePrize() {
    WeaponUpgrade::LoadTexture("pic/red.png", "pic/green.png", "pic/blue.png");
    for (int i = 0; i < 5; i++) {
        Plane *plane;
        plane = new WeaponUpgrade();
        prizeList.InsertBack(plane);
    }
}

void ThunderBolt::gameStart(int key) {
    switch (key) {
        case FSKEY_UP:
            if (game_option == MENU_OPTION_QUIT) {
                game_option--;
            }
            break;
        case FSKEY_DOWN:
            if (game_option == MENU_OPTION_START) {
                game_option++;
            }
            break;
        case FSKEY_ENTER:
            state = game_option;
            break;
        default:
            break;
    }
}

void ThunderBolt::gamePause(int key) {
    switch (key) {
        case FSKEY_UP:
            if (game_option != MENU_OPTION_CONTINUE) {
                game_option--;
            }
            break;
            
        case FSKEY_DOWN:
            game_option = (game_option + 1) % MENU_OPTION_MAX;
            break;
            
        case FSKEY_ENTER:
            if (game_option == MENU_OPTION_CONTINUE) {
                state = GAME_RUNNING;
            }
            else if (game_option == MENU_OPTION_START) {
                resetGame();
                state = GAME_RUNNING;
            }
            else {
                state = GAME_END;
            }
            
        default:
            break;
    }
}

void ThunderBolt::gameRun(int key) {
    int lb, mb, rb, mx, my;
    int mouse = FsGetMouseEvent(lb, mb, rb, mx, my);
    Plane *player = (playerList.getFront())->dat;
    
    if (FSKEY_UP == key) {
        /*UP for power up*/
        ((Thunder *)player)->PowerUp(playerMissleList);
    }
    if (FSMOUSEEVENT_RBUTTONDOWN == mouse) {
        /*Right click for switch between 3 types of weapons*/
        ((Thunder *)player)->upgrade(1, playerMissleList);
    }
    
    /* Thunder: shoot and cool down weapon.*/
    player->Shoot(mouse, playerMissleList);
    player->CoolDown();
    
    /* Thunder: move and draw. */
    player->Move(1.0);
    
    
    /* traverse the missiles list, move missile, and check hit with that
       immortal enemy, just for test CheckHit function */
    MissileNode *node;
    node = playerMissleList.getFront();
    while(node) {
        node->dat->Move(1.0);
        if (!node->dat->CheckInWindow()) {
            node = playerMissleList.Delete(node);
        }
        else {
            node = node->next;
        }
    }
    
    /* traverse the moving enemy list, check hit with all missiles for each
       enemy, delete it if destroyed */
    PlaneNode *pNode;
    pNode = enemyList.getFront();
    while(pNode) {
        pNode->dat->Move(1.0);
        pNode->dat->Aim(player);
        
        if (!pNode->dat->CheckInWindow()) {
            pNode = enemyList.Delete(pNode);
        } else {
            if (pNode->dat->CheckHit(playerMissleList)) {
                pNode = enemyList.Delete(pNode);
            }
            else {
                pNode = pNode->next;
            }
        }
    }
    
    
    /* Traverse the moving prizes */
    pNode = prizeList.getFront();
    while (pNode) {
        pNode->dat->Move(1.0);
        if (((Prize *)(pNode->dat))->Dead()) {
            pNode = prizeList.Delete(pNode);
            Plane *plane;
            plane = new WeaponUpgrade();
            prizeList.InsertBack(plane);
        }
        else {
            pNode = pNode->next;
        }
    }
}

void ThunderBolt::cleanUp() {
    enemyList.CleanUp();
    playerList.CleanUp();
    prizeList.CleanUp();
    enemyMissleList.CleanUp();
    playerMissleList.CleanUp();
}


void ThunderBolt::run() {
    
    FsPollDevice();
    while (!terminate) {
        FsPollDevice();
        int key = FsInkey();
        
        switch (state) {
            case GAME_START:
                gameStart(key);
                break;
            case GAME_RUNNING:
                if (key == FSKEY_ESC) {
                    state = GAME_PAUSE;
                }
                else {
                    gameRun(key);
                }
                break;
            case GAME_PAUSE:
                gamePause(key);
                break;
            case GAME_END:
                terminate = 1;
                break;
        }
        this->draw();
        FsSwapBuffers();
        FsSleep(25);
    }
}

#ifdef GAME_DEBUG

int main() {
    ThunderBolt game;
    game.setup();
    game.run();
    return 0;
}

#endif


