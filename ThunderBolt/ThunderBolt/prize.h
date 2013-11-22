#ifndef THUNDERBOLT_PRIZE_H
#define THUNDERBOLT_PRIZE_H

#include "texture_loader.h"
#include "plane.h"
#include <time.h>

enum PrizeType {
    WEAPON,
    LIFE,
    SHIELD
};

class Prize : public Plane {
    PrizeType pirzeType;

protected:
    /* flash when life < threshold */
    int flashThreshold;
    /* alpha channel: determine transparency */
    int alphaCounter;

    /* surround draw function with these pair to activate flash effect */
    void FlashBegin();
    void FlashEnd();
  
public:
    Prize(PrizeType type);
    void Move(double deltaT);
    int Dead();

};

class WeaponUpgrade : public Prize {
    static TextureLoader bulletTex;
    static TextureLoader cannonTex;
    static TextureLoader laserTex;

    MissileType type;
    /* determines how often does type changes, the larger the slower */
    int typeChangeCap;
    /* counter that controls weapon upgrade type change */
    int typeChangeCounter;

public:
    WeaponUpgrade();
    static int LoadTexture(const char *bullet, const char *cannon, const char *laser);
    void Draw();
    void Move(double deltaT);
    
};

#endif
