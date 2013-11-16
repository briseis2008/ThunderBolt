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

//TODO
void ThunderBolt::drawMenu() {
    glBegin(GL_QUADS);
    if (game_option == MANUE_OPTION_START) {
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
    if (game_option == MANUE_OPTION_QUIT) {
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

void ThunderBolt::draw() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    /* Draw background */
    this->drawBackground();
    
    /* Draw Menu */
    if (state == GAME_START) {
        this->drawMenu();
    }
    
    /* Draw planes */

    /* Draw missiles */
    
}

void ThunderBolt::setup() {
    this->state = GAME_START;
    this->terminate = 0;
    this->game_option = MANUE_OPTION_START;
    
    
}


void ThunderBolt::run() {
    FsOpenWindow(16, 16, WINDOW_WID, WINDOW_HEI, 1);
    FsPollDevice();
    glShadeModel(GL_SMOOTH);
    
    while (!terminate) {
        FsPollDevice();
        int key = FsInkey();
        
        switch (state) {
            case GAME_START:
                switch (key) {
                    case FSKEY_UP:
                        if (game_option == MANUE_OPTION_QUIT) {
                            game_option--;
                        }
                        break;
                    case FSKEY_DOWN:
                        if (game_option == MANUE_OPTION_START) {
                            game_option++;
                        }
                        break;
                    case FSKEY_ENTER:
                        state = game_option;
                        break;
                }
                break;
            case GAME_RUNNING:
                
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
}

#endif


