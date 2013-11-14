//
//  Plane.cpp
//  ThunderBolt
//
//  Created by yigu on 10/31/13.
//  Copyright (c) 2013 CMU. All rights reserved.
//

#include "Plane.h"
#include "missile.h"
#include "debug.h"


Plane::Plane()
{
    this->plane_state = PLANE_NORMAL;
    this->size_x=50;
    this->size_y=50;
    this->position = gZero;
    this->direction = gZero;
    this->velocity = 0.0;
    this->missile = NULL;
    this->missile_level = 1;
    this->missile_reload = 0;
    this->laser = NULL;
    this->firing = 0;
}

Plane::Plane(const Vector2 &position, const Vector2 &direction, int plane_state)
{
    this->plane_state = plane_state;
    this->size_x=50;
    this->size_y=50;
    this->position = position;
    this->direction = direction;
    this->direction.normalize();
    this->velocity = 0.0;
    this->missile = NULL;
    this->missile_level = 1;
    this->missile_reload = 0;
    this->laser = NULL;
    this->firing = 0;
}

Plane::~Plane() {
    if (missile) {
        delete missile;
    }
}



void Plane::Move(double deltaT){
    position += direction * velocity * deltaT;
}


int Plane::CheckHit(Missile *missile)
{
    /*
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
        */
        return 0;
}


int Plane::getPlaneState(){
    return plane_state;
}

void Plane::setPlaneState(int state) {
    this->plane_state = state;
}

void Plane::setVelocity(double velocity) {
    this->velocity = velocity;
}


void Plane::setMissile(MissileType type, const Color &color, int power,
                       const Vector2 &velocity, MissileList &missiles) {
    Missile *newMissile = NULL;
    switch (type) {
        case BULLET:
            newMissile = new Bullet(color, power, gZero, velocity, 1);
            break;
            
        case CANNON:
            newMissile = new Cannon(color, power, gZero, velocity, 1);
            break;
            
        case LASER:
            newMissile = new Laser(color, power, gZero, velocity, 1);
            break;
    }
    assert(newMissile);
    
    if (missile) {
        delete missile;
    }
    
    missile = newMissile;
    missile_level = 1;
    missile_reload = 0;
    
    /* make sure we cancel current laser and reload */
    ReloadLaser(missiles);
}

void Plane::setMissile(Missile *missile, MissileList &missiles) {
    Missile *newMissile = CopyMissile(missile);
    
    if (this->missile) {
        delete this->missile;
    }
    this->missile = newMissile;
    
    missile = newMissile;
    missile_level = 1;
    missile_reload = 0;
    
    /* make sure we cancel current laser and reload */
    ReloadLaser(missiles);   
}


Vector2 Plane::getMissilePos() {
    return position + direction * (size_y / 2);
}

void Plane::ParseAction(int mouse) {
    if (FSMOUSEEVENT_LBUTTONDOWN == mouse) {
        firing = 1;
    } else if (FSMOUSEEVENT_LBUTTONUP == mouse) {
        firing = 0;
    }
}

void Plane::ReloadLaser(MissileList &missiles) {
    if (laser) {
        missiles.Delete(laser);
        laser = NULL;
        missile_reload = 0;
    }
}

void Plane::Shoot(int action, MissileList &missiles){
    ParseAction(action);
    
    if (firing == 1) {
        /* wait for weapon to cool down */
        if (missile_reload > 0) return;

        Vector2 shootPosition = getMissilePos();
    
        Missile *newMissile = CopyMissile(missile);
        newMissile->setPosition(shootPosition);
        newMissile->setDirection(direction);
        
        MissileNode *newNode = missiles.InsertBack(newMissile);
        switch(newMissile->getType()) {
            case BULLET:
                missile_reload = 10;
                break;
                
            case CANNON:
                missile_reload = 20;
                break;
                
            case LASER:
                laser = newNode;
                ((Laser *)newMissile)->setPlane(this);
                missile_reload = 20;
                break;
        }
    } else {
        ReloadLaser(missiles);
    }


    /*
    if(key == FSKEY_J){
        missile.setType(CANNON);
        missile.setColor(255);
        missile.setPosition(shoot_position);
        missile.setVelocity(velocity);
        
        missile.Launch(shoot_position);
        printf("%d\n", missile.getState());
        
    }
    if(key == FSKEY_K) {
        missile.setType(BULLET);
        
        missile.setColor(255);
        missile.setPosition(shoot_position);
        missile.setVelocity(velocity);
        
        missile.Launch(shoot_position);
        
    }
    
    
    
    if(key == FSKEY_L){
        missile.setType(LASER);
        
        missile.setColor(255);
        missile.setPosition(shoot_position);
        
        missile.Launch(shoot_position);
        missile.Draw();
    }

    */
    
}


void Plane::CoolDown(){
    if (missile_reload <= 0) return;
    if (missile->getType() == LASER) return;
    missile_reload--;
}

void Plane::PowerUp() {
    if (missile_level <= MAX_MISSILE_LEVEL) {
        missile_level++;
    }
}

Thunder::Thunder(const Vector2 &position, const Vector2 &direction) 
               : Plane(position, direction, PLANE_NORMAL) {
    life_num = 3;
}


void Thunder::Move(double deltaT) {
    if(FsGetKeyState(FSKEY_A)!=0)
	{
		position.x -= velocity;
	}
	else if(FsGetKeyState(FSKEY_D)!=0)
	{
		position.x += velocity;
	}
    else if(FsGetKeyState(FSKEY_S)!=0)
    {
        position.y += velocity;
    }
    else if(FsGetKeyState(FSKEY_W)!=0){
        position.y -= velocity;
    }
}

void Thunder::Draw(){
    glColor3ub(255,0,0);
    glBegin(GL_POLYGON);
    glVertex2d(position.x,position.y - size_y/2);
    glVertex2d(position.x + size_x / 2, position.y + size_y/2);
    glVertex2d(position.x - size_x / 2, position.y + size_y/2);
    glEnd();
    
}

void Enemy1::Draw() {
    glColor3ub(255,0,0);
    glBegin(GL_QUADS);
    glVertex2d(position.x,position.y);
    glVertex2d(position.x,position.y-50);
    glVertex2d(position.x+50,position.y-50);
    glVertex2d(position.x+50,position.y);
    glEnd();
}


#ifdef PLANE_DEBUG
int main(void){
    Vector2 startPosition(300, 280);
    Vector2 startDirection(0, -1);
    
    MissileList missiles;
    Missile *sample[3];
    sample[0] = new Bullet(gRed, 100, gZero, Vector2(0, -20), 1);
    sample[1] = new Cannon(gGreen, 100, gZero, Vector2(0, -10), 1);
    sample[2] = new Laser(gBlue, 100, gZero, Vector2(0, -1), 1);
    
    Plane *plane;
    Thunder thunder(startPosition, startDirection);
    thunder.setVelocity(5);
    thunder.setMissile(LASER, gBlue, 100, Vector2(0, -1), missiles);
    plane = &thunder;
    
    FsOpenWindow(0,0,800,600,1);
    
    bool running = true;
    int i = 0;
    
    while(running)
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        FsPollDevice();
        
        int key=FsInkey();
        int lb, mb, rb, mx, my;
        int mouse = FsGetMouseEvent(lb, mb, rb, mx, my);

        if(FSKEY_ESC==key)
        {
            running=false;
            break;
        }
        if (FSMOUSEEVENT_RBUTTONDOWN == mouse) {
            plane->setMissile(sample[i], missiles);
            i = (i+1) % 3;
        }
        
        plane->Shoot(mouse, missiles);
        plane->CoolDown();
        
        plane->Move(1.0);
        plane->Draw();
        
        
        MissileNode *node;
        node = missiles.getFront();
        while(node) {
            node->dat->Move();
            node->dat->Draw();
            if (!node->dat->CheckInWindow()) {
                node = missiles.Delete(node);
            } else {
                node = node->next;
            }
        }
        
        
        
        FsSwapBuffers();
        FsSleep(25);
    }
    
    return 0;
}
#endif



