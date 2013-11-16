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
    this->life = 0;
}

Plane::Plane(const Vector2 &position, const Vector2 &direction, int plane_state, int life)
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
    this->life = life;
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
    double dist;
    switch (missile->getType()) {
        /* for performance, treat bullet as a single point */
        case BULLET:
            dist = position.distance(missile->position);
            if (dist <= size_x / 2) return 1;
            break;
            
        case CANNON:
            dist = position.distance(missile->position);
            if (dist <= size_x / 2 + ((Cannon *)missile)->radius) return 1;            
            break;
            
        case LASER:
            double a = missile->direction.y;
            double b = -missile->direction.x;
            double c = missile->direction.x * missile->position.y 
                     - missile->direction.y * missile->position.x;
            /* distance from point to line */
            dist = fabs(a * position.x + b * position.y + c)
                 / sqrt(a * a + b * b);
            if (dist <= size_x / 2 + ((Laser *)missile)->width) return 1;
            break;
    }
    
    return 0;
}

/* check hit with all missiles, return 1 if destroyed by missiles; 0 otherwise*/
int Plane::CheckHit(MissileList &missiles) {
    MissileNode *node = missiles.getFront();
    while (node) {
        if (CheckHit(node->dat)) {
            life -= node->dat->power;
            
            if (node->dat->getType() != LASER) 
                node = missiles.Delete(node);
            else 
                node = node->next;
                        
            if (life <= 0) return 1;
        } else {
            node = node->next;            
        }
    }
    
    return 0;
}

int Plane::CheckHit(Plane *plane) {
    double dist;
    dist = position.distance(plane->position);
    if (dist <= size_x / 2 + plane->size_x / 2) return 1;
    
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

int Plane::CheckInWindow() {
    if (position.x >= 0 && position.x < WINDOW_WID
     && position.y >= 0 && position.y < WINDOW_HEI)
        return 1;
        
    return 0;
}

Thunder::Thunder(const Vector2 &position, const Vector2 &direction) 
               : Plane(position, direction, PLANE_NORMAL, 1000) {
    life_num = 3;
}


void Thunder::Move(double deltaT) {
    if(FsGetKeyState(FSKEY_A)!=0)
	{
		position.x -= velocity;
	}
	if(FsGetKeyState(FSKEY_D)!=0)
	{
		position.x += velocity;
	}
    if(FsGetKeyState(FSKEY_S)!=0)
    {
        position.y += velocity;
    }
    if(FsGetKeyState(FSKEY_W)!=0){
        position.y -= velocity;
    }
    
    /* you wanna move thunder out of screen? NO WAY! Muahahahaha */
    if (position.x < 0) position.x = 0;
    if (position.x > WINDOW_WID) position.x = WINDOW_WID;
    if (position.y < size_y / 2) position.y = size_y / 2;
    if (position.y > WINDOW_HEI - size_y / 2) position.y = WINDOW_HEI - size_y / 2;
}

void Thunder::Draw(){
    glColor3ubv(gRed.arr());
    glBegin(GL_POLYGON);
    glVertex2d(position.x,position.y - size_y/2);
    glVertex2d(position.x + size_x / 2, position.y + size_y/2);
    glVertex2d(position.x - size_x / 2, position.y + size_y/2);
    glEnd();
    
}

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
    }


#ifdef PLANE_DEBUG
int main(void){
    Vector2 startPosition(300, 280);
    Vector2 startDirection(0, -1);
    
    MissileList missiles;
    Missile *sample[3];
    sample[0] = new Bullet(gRed, 300, gZero, Vector2(0, -20), 1);
    sample[1] = new Cannon(gGreen, 1000, gZero, Vector2(0, -10), 1);
    sample[2] = new Laser(gBlue, 20, gZero, Vector2(0, -1), 1);
    
    Plane *plane;
    Thunder thunder(startPosition, startDirection);
    thunder.setVelocity(5);
    thunder.setMissile(sample[0], missiles);
    plane = &thunder;
    
    Plane *enemy;
    Enemy1 enemy1(Vector2(200, 100), Vector2(0,1));
    enemy = &enemy1;
    
    PlaneList enemies;
    
    FsOpenWindow(0,0,WINDOW_WID,WINDOW_HEI,1);
    glClearColor(0.1, 0.1, 0.1, 1);
    
    bool running = true;
    int i = 0;
    int j = 0;
    
    while(running)
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        FsPollDevice();
        
        int key=FsInkey();
        int lb, mb, rb, mx, my;
        int mouse = FsGetMouseEvent(lb, mb, rb, mx, my);

        if (j % 100 == 0) {
            Plane *newP = new Enemy1(Vector2(0, 0), Vector2(1, 1));
            newP->setVelocity(2.0);
            enemies.InsertBack(newP);
        }
        j++;
        
        if(FSKEY_ESC==key)
        {
            running=false;
            break;
        }
        if (FSMOUSEEVENT_RBUTTONDOWN == mouse) {
            i = (i+1) % 3;
            plane->setMissile(sample[i], missiles);
        }
        
        plane->Shoot(mouse, missiles);
        plane->CoolDown();
        
        plane->Move(1.0);
        plane->Draw();
        
        enemy->Draw();
        
        
        MissileNode *node;
        node = missiles.getFront();
        while(node) {
            node->dat->Move(1.0);
            
            if (!node->dat->CheckInWindow()) {
                node = missiles.Delete(node);
            } else if (enemy->CheckHit(node->dat)) {
                printf("Hit by missile!\t%p\t%p\n", node, node->dat);
                /* only delete when not laser */
                if (node->dat->getType() != LASER) node = missiles.Delete(node);
                else node = node->next;
            } else {
                node = node->next;
            }
        }

        PlaneNode *pNode;
        pNode = enemies.getFront();
        while(pNode) {
            pNode->dat->Move(1.0);
            
            if (!pNode->dat->CheckInWindow()) {
                pNode = enemies.Delete(pNode);
            } else {
                if (pNode->dat->CheckHit(missiles))
                    pNode = enemies.Delete(pNode);
                else
                    pNode = pNode->next;
            }
        }

        FOR_EACH(pNode, enemies) {
            pNode->dat->Draw();
        }
        FOR_EACH(node, missiles) {
            node->dat->Draw();
        }
        
        
        FsSwapBuffers();
        FsSleep(25);
    }
    
    return 0;
}
#endif



