#include "boss.h"

void BossLeftArm::Draw() {
    glColor3ub(100, 0, 200);
    
    glBegin(GL_POLYGON);
    glVertex2d(position.x + direction.x * size_y/2,
               position.y + direction.y * size_y/2);
    glVertex2d(position.x - direction.x * size_y/2 + direction.y * size_x/2,
               position.y - direction.y * size_y/2 - direction.x * size_x/2);
    glVertex2d(position.x - direction.x * size_y/2 - direction.y * size_x/2,
               position.y - direction.y * size_y/2 + direction.x * size_x/2);
    glEnd();
}

void BossRightArm::Draw() {
    glColor3ub(100, 100, 0);
    
    glBegin(GL_POLYGON);
    glVertex2d(position.x + direction.x * size_y/2,
               position.y + direction.y * size_y/2);
    glVertex2d(position.x - direction.x * size_y/2 + direction.y * size_x/2,
               position.y - direction.y * size_y/2 - direction.x * size_x/2);
    glVertex2d(position.x - direction.x * size_y/2 - direction.y * size_x/2,
               position.y - direction.y * size_y/2 + direction.x * size_x/2);
    glEnd();
}

void Boss::Draw() {
    glColor3ub(100, 100, 100);
    
    glBegin(GL_POLYGON);
    int i;
    for(i=0; i<64; i++)
    {
        double angle=(double)i*PI/32.0;
        double x=position.x + cos(angle)*size_x / 2;
        double y=position.y + sin(angle)*size_x / 2;
        glVertex2d(x,y);
    }

    glEnd();
    
    leftArm.Draw();
    rightArm.Draw();
}

void Boss::Aim(Plane *target) {
    leftArm.Aim(target);
    rightArm.Aim(target);
}
