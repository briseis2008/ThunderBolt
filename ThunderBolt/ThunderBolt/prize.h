#ifndef THUNDERBOLT_PRIZE_H
#define THUNDERBOLT_PRIZE_H

#include "plane.h"

class WeaponUpdate : public Plane {
    MissileType type;
    
public:
    WeaponUpdate(MissileType type) : type(type) {};
};

#endif
