#include "prize.h"

Prize::Prize()
{
    position.x = position.x + p_x0;
    p_sizex = 20;
    p_sizey = 20;
    this->position = Vector(400,300);
    this->velocity = Vector(2,2);
    this->prize_state = 1;
    this->prize_lifetime = 1000;
    
   
}
void Prize::Draw()
{
    glColor3ub(0,255,0);
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
    if ((position.x < 0 && velocity.x < 0)|| (position.x+p_sizex > 800 && velocity.x > 0))
    {
        velocity.x = -velocity.x;
    }
    if ((position.y-p_sizey < 200 && velocity.y < 0)|| (position.y > 500 && velocity.y > 0) )
    {
        velocity.y = -velocity.y;
    }
}

void Prize::Disappear()
{
    prize_lifetime = prize_lifetime - 1;
    if (prize_lifetime == 0)
    {
        prize_state = 0;
    }
}
int main(void)
{
    Prize prize;
    
    FsOpenWindow(0,0,800,600,1);
    
    bool running = true;
    
    while(running)
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        FsPollDevice();
        prize.Disappear();
        if (prize.prize_state != 0)
            {
                prize.Draw();
                prize.Move();

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
