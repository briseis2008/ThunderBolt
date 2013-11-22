#include "prize.h"
#include "missile.h"
#include "global.h"
#include "debug.h"


Prize::Prize(PrizeType type) : pirzeType(type) {
    /* prize floating speed */
    velocity = 5;
    /* random starting position and direction */
    position = Vector2(rand()%(int)WINDOW_WID, rand()%(int)WINDOW_HEI);
    direction = Vector2(rand()%100 - 50, rand()%100 - 50);
    direction.normalize();
    
    flashThreshold = 300;
    /* non transparent */
    alphaCounter = 255;
}

/* move prize according to current speed, and change direction randomly */
void Prize::Move(double deltaT){
    /* deltaS = v * deltaT */
    position += (direction * velocity) * deltaT;
    
    /* bounce back so it will not move out of window */
    if (position.x <= size_x / 2) {
        position.x = size_x / 2;
        direction.x = -direction.x;
    } else if (position.x >= WINDOW_WID - size_x / 2) {
        position.x = WINDOW_WID - size_x / 2;
        direction.x = -direction.x;
    }
    if (position.y <= size_y / 2) {
        position.y = size_y / 2;
        direction.y = -direction.y;
    } else if (position.y >= WINDOW_HEI - size_y / 2) {
        position.y = WINDOW_HEI - size_y / 2;
        direction.y = -direction.y;
    }
    
    /* rotate direction a little bit, 
       try to rotate counter-clockwise with some random variation */
    double rad = 0.01;
    rad += (rand()%100 - 50) / 1000.0;
    direction.rotate(rad);
}

/* if this prize is dead, return 1. Otherwise return 0 */
int Prize::Dead() {
    if (life <= 0) return 1;
    
    return 0;
}

void Prize::FlashBegin() {
    /* flash the weapon */
    if (life < flashThreshold) {
        /* GL_MODULATE means: final color = texture color * primative color */
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        /* so this value will change between 0 -> 255 -> 0 -> 255 -> ... */
        int alpha = abs(alphaCounter % 510 - 255);
        glColor4ub(255, 255, 255, alpha);
    } 
}

void Prize::FlashEnd() {
    /* update alpha, and restore normal texture environment setting */
    if (life < flashThreshold) {
        alphaCounter +=10;
        glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    }    
}


TextureLoader WeaponUpgrade::bulletTex;
TextureLoader WeaponUpgrade::cannonTex;
TextureLoader WeaponUpgrade::laserTex;

WeaponUpgrade::WeaponUpgrade() : Prize(WEAPON) {
    size_x = 32;
    size_y = 32;
    life = 1000;
    /* random weapon upgrade type */
    type = (MissileType)(rand() % 3);
    typeChangeCap = 200;
    typeChangeCounter = 0;
}

/* load texture for weapon upgrade. Return 0 is succeed, 1 otherwise */
int WeaponUpgrade::LoadTexture(const char *bullet, const char *cannon, 
                               const char *laser) {
    int result = 0;
    result |= bulletTex.LoadTexture(bullet);
    result |= cannonTex.LoadTexture(cannon);
    result |= laserTex.LoadTexture(laser);
    
    return result;
}

void WeaponUpgrade::Draw() {
    /* flash effect is taken care of by Prize */
    FlashBegin();
    /* general draw function */
    switch (type) {
        case BULLET:
            bulletTex.Draw(position, Vector2(0,-1), size_x, size_y);
            break;
            
        case CANNON:
            cannonTex.Draw(position, Vector2(0,-1), size_x, size_y);
            break;
            
        case LASER:
            laserTex.Draw(position, Vector2(0,-1), size_x, size_y);
            break;
    }
    FlashEnd();
}

/* move current weapon upgrade, and change life and type according to time */
void WeaponUpgrade::Move(double DeltaT) {
    /* regular prize move */
    Prize::Move(DeltaT);
    
    /* so it's dying as time passed by */
    life -= 1;
    /* change weapon upgrade type whenever typeCounter hits typeChangeCap */
    if (++typeChangeCounter == typeChangeCap) typeChangeCounter = 0;
    if (0 == typeChangeCounter) type = (MissileType)((type + 1) % 3);
}


#ifdef PRIZE_DEBUG
int main () {
    srand(time(NULL));
    FsOpenWindow(0,0,600, 800,1);
    glClearColor(0, 0, 0, 1);    
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    textureInit();
    
    WeaponUpgrade::LoadTexture("pic/red.png", "pic/green.png", "pic/blue.png");
    Plane *plane;
    plane = new WeaponUpgrade();
    bool running = true;

    while(running)
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        FsPollDevice();

        int key=FsInkey();
        int lb, mb, rb, mx, my;
        int mouse = FsGetMouseEvent(lb, mb, rb, mx, my);

        if(FSKEY_ESC==key)
        {
            running=false;
        }
        
        plane->Draw();
        plane->Move(1.0);
        if (((Prize *)plane)->Dead()) {
            delete plane;
            plane = new WeaponUpgrade();
        }
        
        FsSwapBuffers();
        FsSleep(25);
        
    }   
    return 0;
}
#endif
