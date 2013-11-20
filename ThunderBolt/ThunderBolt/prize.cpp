/*
 Thunderbolt prize.cpp edition 2
 Created by Yan Zhang
 Andrew ID: yanz3
 */
#include "prize.h"
#include "Plane.h"
#include "debug.h"
#include <time.h>
#include <stdlib.h>

Prize::Prize()
{
    position.x = position.x + p_x0;
    this->p_sizex = 20;
    this->p_sizey = 20;
    for (int i = 0; i < 4; i++)
    {
        this->position = Vector2(rand()%50+i*100, rand()%70+i*100);
        
        this->velocity = Vector2(rand()%1+i,4%1+i);
    }
    this->prize_state = 1;
    this->prize_lifetime = 1000;
    this->nprize = 4;
    //bool c = false;// to check whether the prize is caught by plane
        
}
void Prize::Draw()
{
    glBegin(GL_QUADS);
    glVertex2d(position.x,position.y);
    glVertex2d(position.x,position.y-p_sizey);
    glVertex2d(position.x+p_sizex,position.y-p_sizey);
    glVertex2d(position.x+p_sizex,position.y);
    glEnd();
}
void Prize::Move()
{
    position.x+=velocity.x;
    position.y+=velocity.y;
    if ((position.x < 0 && velocity.x < 0)|| (position.x+p_sizex > 600 && velocity.x > 0))
    {
        velocity.x = -velocity.x;
    }
    if ((position.y-p_sizey < 200 && velocity.y < 0)|| (position.y > 600 && velocity.y > 0) )
    {
        velocity.y = -velocity.y;
    }
}

//void Prize::setType(MissileType &type)
//{
//    //return MissileType
//    this->type = LIFE;
//    this->type = BULLET;
//    this->type = CANNON;
//    this->type = LASER;
//}

//int Prize::CheckCatchByPlane(Plane &plane)
//{
//
//    for (int i = 0; i < 4; i++)
//    {
//        if (position.x - plane.size_x < 0 && position.y - plane.size_y < 0)
//        {
//            c = true;
//            return i;
//        }
//    }
//
//}


void Prize::Disappear()
{
    prize_lifetime = prize_lifetime - 1;
    if (prize_lifetime == 0)
    {
        prize_state = 0;
    }
}

#ifdef DEGUG_PRIZE
int main(void)
{
    Prize prize[4];
    
    FsOpenWindow(0,0,600,800,1);
    
    bool running = true;
    
    while(running)
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        FsPollDevice();
        for (int i = 0; i < 4; i++)
        {
            prize[i].Disappear();
            if (prize[i].prize_state != 0)
            {
                glColor3ub(rand()%251+i,rand()%251+i,rand()%251+i);
                prize[i].Draw();
                prize[i].Move();
                //prize[i].CheckCatchByPlane(<#Plane &plane#>);
                
            }

        }
        int key=FsInkey();
        if(FSKEY_ESC==key)
        {
            running=false;
            break;
        }
        FsSwapBuffers();
        FsSleep(25);
    }
    
    
    return 0;
    
}
#endif
