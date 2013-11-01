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
    int size_x, size_y;
    Vector position;
    Vector velocity;
    int missile_state;
    Missile missile;
    
public:
    Plane();
    
    Plane(int plane_state, Vector position, Vector velocity, int missile_state);
    
    void Draw();
    
    int getPlaneState();
    
    void Move();
    
    void Disappear(void);
    
    int CheckHit(Missile missile);
    
    Missile getMissile();
    
    void Shoot(int key);
    
    void setMissile(MissileType type,int color, Vector position, Vector velocity);
    
};



#endif