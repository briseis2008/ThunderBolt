#ifndef THUNDERBOLT_BOSS_H
#define THUNDERBOLT_BOSS_H

#include "plane.h"


/* The idea of boss:
 * It could contain member of other inheritance classes, 
 * such that each component of boss could aim and shoot on
 * its own without interference. Modify debug.h to only define PLANE_DEBUG to
 * see a demo of this none-moving super dumb boss.
 *
 * This is just an demonstration of boss. 
 * Feel free to implement your own version! */

class BossLeftArm : public Plane {
public:
    BossLeftArm (const Vector2 &position, const Vector2 &direction)
         : Plane(position, direction, PLANE_NORMAL, 50, 50, 1000) {};  
    virtual void Draw();
};

class BossRightArm : public Plane {
public:
    BossRightArm (const Vector2 &position, const Vector2 &direction)
         : Plane(position, direction, PLANE_NORMAL, 50, 50, 1000) {};  
    virtual void Draw();
};

class Boss : public Plane{
    BossLeftArm leftArm;
    BossRightArm rightArm;
public:
    Boss(const Vector2 &position, const Vector2 &direction)
    : Plane(position, direction, PLANE_NORMAL, 200, 200, 1000),
      leftArm(Vector2(-75, 75) + position, direction),
      rightArm(Vector2(75, 75) + position, direction) {};
    virtual void Draw();
    virtual void Aim(Plane *target);
};

#endif
