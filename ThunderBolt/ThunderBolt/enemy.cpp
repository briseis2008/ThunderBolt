#include "enemy.h"

void Enemy1::Draw() {
    glColor3ub(200, 200, 200);
    
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
    
    
    /* aiming line */
    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);
    glVertex2d(position.x, position.y);
    Vector2 aimPoint = position + direction * size_y;
    glVertex2d(aimPoint.x, aimPoint.y);
    glEnd();
}

