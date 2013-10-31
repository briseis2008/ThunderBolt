#include "missile.h"

Vector Missile::getPosition()
{
    return position;
}

void Missile::Launch(Vector position)
{
    if(state == 0)
    {
        if(type == LASER)
            countDown = 100;
        state = 1;
        this->position = position;
    }
}

Missile::Missile(MissileType type, int color, int power, int shootMode, Vector position, Vector velocity)
{
    state = 0;
    this->type = type;
    this->color = color;
    this->red = (color>>16)&0xff;
    this->green = (color>>8)&0xff;
    this->blue = color&0xff;
    this->power = power;
    this->shootMode = shootMode;
    this->position = position;
    this->velocity = velocity;
}

Missile::Missile(MissileType type, int color, int power, int shootMode, Vector position)
{
    state = 0;
    this->type = type;
    this->color = color;
    this->red = (color>>16)&0xff;
    this->green = (color>>8)&0xff;
    this->blue = color&0xff;
    this->power = power;
    this->shootMode = shootMode;
    this->position = position;
}

Missile::Missile(MissileType type, int color, Vector position)
{
    state = 0;
    this->type = type;
    this->color = color;
    this->red = (color>>16)&0xff;
    this->green = (color>>8)&0xff;
    this->blue = color&0xff;
    this->position = position;
}

Missile::Missile()
{
    state = 0;
    this->color = 0;
    this->red = (color>>16)&0xff;
    this->green = (color>>8)&0xff;
    this->blue = color&0xff;
    this->power = 0;
    this->shootMode = 0;
    this->position = Vector(0,0);
    this->velocity = Vector(0,0);
}

int Missile::CheckInWindow(void)
{
    if(type == LASER)
    {
        countDown -= 5;
        if(countDown < 0)
            state = 0;
    }
    else if(position.y<0 || position.y > 600)
    {
        state = 0;
    }
    else
    {
        state = 1;
    }
    return state;
}

void Missile::Draw(void)
{
    if (state)
    {
        switch(type)
        {
        case BULLET:
            glColor3ub(red,green,blue);
            glBegin(GL_LINES);
            glVertex2d(position.x,position.y);
            glVertex2d(position.x,position.y-10);
            glEnd();
            break;
        case LASER:
            glShadeModel(GL_SMOOTH);

            glBegin(GL_QUADS);

            glColor3ub(red,green,blue);
            glVertex2d(position.x,      position.y);

            glColor3ub(red,green,blue);
            glVertex2d(position.x,      0);

            glColor3ub(255,255,255);
            glVertex2d(position.x + 10, 0);

            glColor3ub(255,255,255);
            glVertex2d(position.x + 10, position.y);


            glColor3ub(red,green,blue);
            glVertex2d(position.x + 20, position.y);

            glColor3ub(red,green,blue);
            glVertex2d(position.x + 20, 0);

            glColor3ub(255,255,255);
            glVertex2d(position.x + 10, 0);

            glColor3ub(255,255,255);
            glVertex2d(position.x + 10, position.y);

            glEnd();
            break;
        case CANNON:

            glBegin(GL_POLYGON);

            glColor3ub(red,green,blue);
            int i;
            for(i=0; i<64; i++)
            {
                double angle=(double)i*YS_PI/32.0;
                double x=position.x+cos(angle)*(double)10;
                double y=position.y+sin(angle)*(double)10;
                glVertex2d(x,y);
            }

            glEnd();
            break;

        }
    }
}

void Missile::Move(Vector newPosition)
{
    position = newPosition;
}

void Missile::Move()
{
    position.y += velocity.y;
    position.x += velocity.x;
}

int main(void)
{
    Vector bulletV(0, -10);
    Vector cannonV(0, -20);
    Vector bulletPosition(300, 400);
    Vector laserPosition(100, 400);
    Vector cannonPosition(200, 400);
    Vector velocity(1, 1);
    Missile laser(LASER, 255, 1, 1, laserPosition);
    Missile bullet(BULLET, 255<<16, 1, 1, bulletPosition, bulletV);
    Missile cannon(CANNON, (255<<8)+255, 1, 1, cannonPosition, cannonV);

    FsOpenWindow(0,0,800,600,1);

    bool running = true;

    while(running)
    {


        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        FsPollDevice();

        int key=FsInkey();
        if(FSKEY_ESC==key)
        {
            running=false;
        }
        else if(FSKEY_UP==key)
        {

        }
        else if(FSKEY_DOWN==key)
        {

        }
        else if(FSKEY_SPACE==key)
        {
            laser.Launch(laserPosition);
            bullet.Launch(bulletPosition);
            cannon.Launch(cannonPosition);

        }

        bullet.Move();
        cannon.Move();
        cannon.CheckInWindow();
        bullet.CheckInWindow();
        laser.CheckInWindow();
        cannon.Draw();
        bullet.Draw();
        laser.Draw();
        FsSwapBuffers();


        FsSleep(25);
    }

    return 0;
}
