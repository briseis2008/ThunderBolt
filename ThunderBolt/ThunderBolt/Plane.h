#ifndef __ThunderBolt__Plane__
#define __ThunderBolt__Plane__

#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "missile.h"

class Plane
{
protected:
    int plane_state;
    int life_num;
    Vector position;
    Vector velocity;
    int missile_state;
    Missile missile;
    
public:
    Plane();
    
    Plane(int plane_state, Vector position, Vector velocity, int missile_state);
    
    void Move(int key);
    
    void Disappear();
    
    int CheckHit(Vector comingPosition);
    
    void Shoot(Vector startPosition);
    
};



#endif