//
//  Plane.cpp
//  ThunderBolt
//
//  Created by yigu on 10/31/13.
//  Copyright (c) 2013 CMU. All rights reserved.
//

#include "Plane.h"
#include "missile.h"

Plane::Plane()
{
    this->plane_state = 1;
    this->life_num = 3;
    this->position = Vector(0,0);
    this->velocity = Vector(0,0);
    this->missile_state = 0;
    // need to set missile according to state, like this->missile = Missile.set(missile_state);
}

Plane::Plane(int plane_state, Vector position, Vector velocity, int missile_state)
{
    this->plane_state = plane_state;
    this->position = position;
    this->velocity = velocity;
    this->missile_state = missile_state;
    // the same as above function
}

void Plane::Draw(){
    glColor3ub(255,0,0);
    glBegin(GL_QUADS);
    glVertex2d(position.x,position.y);
    glVertex2d(position.x,position.y-50);
    glVertex2d(position.x+50,position.y-50);
    glVertex2d(position.x+50,position.y);
    glEnd();
    
}

void Plane::Move(){
    if(FsGetKeyState(FSKEY_A)!=0)
	{
		position.x -= velocity.x;
	}
	else if(FsGetKeyState(FSKEY_D)!=0)
	{
		position.x += velocity.x;
	}
    else if(FsGetKeyState(FSKEY_S)!=0)
    {
        position.y += velocity.y;
    }
    else if(FsGetKeyState(FSKEY_W)!=0){
        position.y -= velocity.y;
    }
}

void Plane::Disappear(void){
    this->plane_state = 0;
    this->life_num--;
}

int Plane::CheckHit(Missile missile)
{
	if(missile.getType() == BULLET){
		if(missile.getPosition().x>=position.x && missile.getPosition().x<=position.x+size_x && missile.getPosition().y>=position.y-size_y && missile.getPosition().y-10<=position.y)
			return 1;
		else
			return 0;
	}
	else if(missile.getType() == CANNON){
		if(missile.getPosition().x+10>=position.x && missile.getPosition().x-10<=position.x+size_x && missile.getPosition().y+10>=position.y-size_y && missile.getPosition().y-10<=position.y)
			return 1;
		else
			return 0;
	}
	else if(missile.getType() == LASER){
		if(missile.getPosition().x+20>=position.x && missile.getPosition().x<=position.x+size_x && missile.getPosition().y>=position.y-size_y)
			return 1;
		else
			return 0;
	}else
        return 0;
}


int Plane::getPlaneState(){
    return plane_state;
}

void Plane::Shoot(int key){
    if(FsGetKeyState(FSKEY_J)!=0){
        Vector velocity(0, -10);
        setMissile(CANNON, 255, position, velocity);
        
        missile.Launch(position);

    }
    else if(FsGetKeyState(FSKEY_K)!=0){
        Vector velocity(0, -10);
        setMissile(BULLET, 255, position, velocity);
        missile.Launch(position);
        
    }
    
    missile.Move();
    missile.Draw();
    
}

void Plane::setMissile(MissileType type,int color, Vector position, Vector velocity){
    missile.setType(type);
    missile.setColor(color);
    missile.setPosition(position);
    missile.setVelocity(velocity);
}

Missile Plane::getMissile(){
    return missile;
}



int main(void){
    Vector startPoisiton(20, 580);
    Vector startSpeed(10, 10);
    
    Plane plane(1, startPoisiton, startSpeed, 0);
    
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
            break;
        }
        
        if(plane.getPlaneState() == 1){
            plane.Draw();
        }
        
        
        plane.Move();
        
        plane.Shoot(key);
        
       
        
        
        FsSwapBuffers();
        FsSleep(25);
    }
    
    return 0;
}



