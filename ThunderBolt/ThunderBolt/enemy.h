#ifndef THUNDERBOLT_ENEMY_H
#define THUNDERBOLT_ENEMY_H

#include "plane.h"


/* Some dummy stupid enemy who only knows how to move slowly */
class Enemy1 : public Plane {
public:
    Enemy1(const Vector2 &position, const Vector2 &direction)
         : Plane(position, direction, PLANE_NORMAL, 50, 50, 1000) {};  
    virtual void Draw();
};


#endif
