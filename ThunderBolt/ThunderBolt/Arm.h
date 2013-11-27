//
//  Arm.h
//  ThunderBolt
//
//  Created by yigu on 11/26/13.
//  Copyright (c) 2013 CMU. All rights reserved.
//

#ifndef __ThunderBolt__Arm__
#define __ThunderBolt__Arm__

#include "Plane.h"

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

#endif /* defined(__ThunderBolt__Arm__) */
