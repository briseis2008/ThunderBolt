//
//  Arm.cpp
//  ThunderBolt
//
//  Created by yigu on 11/26/13.
//  Copyright (c) 2013 CMU. All rights reserved.
//

#include "Arm.h"

void Arm::Draw() {
    glColor3ub(color.r(), color.g(), color.b());
    
    glBegin(GL_POLYGON);
    glVertex2d(position.x + direction.x * size_y/2,
               position.y + direction.y * size_y/2);
    glVertex2d(position.x - direction.x * size_y/2 + direction.y * size_x/2,
               position.y - direction.y * size_y/2 - direction.x * size_x/2);
    glVertex2d(position.x - direction.x * size_y/2 - direction.y * size_x/2,
               position.y - direction.y * size_y/2 + direction.x * size_x/2);
    glEnd();
}

void Arm::Init(MissileList &missiles) { }

void Arm::SetPosition(const Vector2 &newPosition) {
    position = newPosition;
}
