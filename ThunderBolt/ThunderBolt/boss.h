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

class Arm : public Plane {
    Color color;
public:
    Arm (const Vector2 &position, const Vector2 &direction, Color color)
    : Plane(position, direction, PLANE_NORMAL, 50, 50, 1000) {
        this->color = color;
    };
    virtual void Init(MissileList &missiles);
    virtual void Draw();
    virtual void SetPosition(const Vector2 &position);
};

class BossLeftArm : public Arm {
public:
    BossLeftArm (const Vector2 &position, const Vector2 &direction)
    : Arm(position, direction, Color(100, 0, 100)) {};
    virtual void Init(MissileList &missiles);
};

class BossRightArm : public Arm {
public:
    BossRightArm (const Vector2 &position, const Vector2 &direction)
    : Arm(position, direction, Color(100, 100, 0)) {};
    virtual void Init(MissileList &missiles);
};

class BossMidArm : public Arm {
public:
    BossMidArm (const Vector2 &position, const Vector2 &direction)
    : Arm(position, direction, Color(100, 100, 100)) {};
    virtual void Init(MissileList &missiles);
    virtual void ReloadLaser(MissileList &missiles);
};

class Boss : public Plane{
    BossLeftArm leftArm;
    BossRightArm rightArm;
    BossMidArm  midArm;
    bool wander = false;
    bool alive = true;
public:
    Boss(const Vector2 &position, const Vector2 &direction)
    : Plane(position, direction, PLANE_NORMAL, 200, 200, 30000),
      leftArm(Vector2(-75, 75) + position, direction),
      rightArm(Vector2(75, 75) + position, direction),
      midArm(Vector2(0, 75) + position, direction) {};
    
    virtual void Init(MissileList &missiles);
    virtual void Draw();
    virtual void Aim(Plane *target);
    virtual void Shoot(MissileList &missiles);
    virtual void CoolDown();
    virtual void Move(double deltaT);
    
    virtual int getLife() const;
    virtual int CheckHit(MissileList &missiles);
    virtual void Disappear(MissileList &missiles);
};

#endif
