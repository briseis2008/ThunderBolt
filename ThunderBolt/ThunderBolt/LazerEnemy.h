//
//  LazerEnemy.h
//  ThunderBolt
//
//  Created by yigu on 11/26/13.
//  Copyright (c) 2013 CMU. All rights reserved.
//

#ifndef __ThunderBolt__LazerEnemy__
#define __ThunderBolt__LazerEnemy__

#include "Plane.h"
#include "Arm.h"

/*
 * Only	emit laser,	move fast,	and	with higher life value.
 *
 */

class LazerArm : public Arm {
public:
    LazerArm (const Vector2 &position, const Vector2 &direction)
    : Arm(position, direction, Color(100, 100, 100)) {};
    virtual void Init(MissileList &missiles);
    virtual void ReloadLaser(MissileList &missiles);
};

class LazerEnemy: public Plane
{
    LazerArm arm;
    bool wander = false;
    bool alive = true;
public:
    LazerEnemy(const Vector2 &position, const Vector2 &direction)
    : Plane(position, direction, PLANE_NORMAL, 100, 100, 3000),
    arm(Vector2(0, 75) + position, direction) {};
    
    virtual void Init(MissileList &missiles);
    virtual void Draw();
    virtual void Shoot(MissileList &missiles);
    virtual void CoolDown();
    virtual void Move(double deltaT);
    
    virtual int getLife() const;
    virtual int CheckHit(MissileList &missiles);
    virtual void Disappear(MissileList &missiles);
};


#endif /* defined(__ThunderBolt__LazerEnemy__) */
