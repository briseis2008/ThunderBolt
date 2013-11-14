#ifndef __ThunderBolt__Plane__
#define __ThunderBolt__Plane__

#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "missile.h"


#define PLANE_NORMAL 1
#define PLANE_SUPER  2

#define MAX_MISSILE_LEVEL 5

class Plane
{
protected:
    int size_x, size_y;
    
    Vector2 position;
    Vector2 direction;
    double velocity;
    
    Missile *missile;
    int missile_level;
    int missile_reload;
    
    /* keep track of the laser we shoot so we could stop it when needed */
    MissileNode *laser;
    
    int plane_state;
    int firing;
    
    void ParseAction(int mouse);
    void ReloadLaser(MissileList &missiles);
    Vector2 getMissilePos();
    
public:
    Plane();    
    Plane(const Vector2 &position, const Vector2 &direction, int plane_state);
    virtual ~Plane();
    
    virtual void Draw() = 0;
    
    int getPlaneState();
    void setPlaneState(int state);
    void setVelocity(double velocity);
    
    virtual void Move(double deltaT);

    void Shoot(int action, MissileList &missiles);
    void CoolDown();
    
    int CheckHit(Missile *missile);
    
    void setMissile(MissileType type, const Color &color, int power,
                    const Vector2 &velocity, MissileList &missiles);
    void setMissile(Missile *missile, MissileList &missiles);
                    
    void PowerUp();
    
    friend class Laser;
};

class Thunder : public Plane {
protected:
    int life_num; 
public:
    Thunder(const Vector2 &position, const Vector2 &direction);
    void Draw();
    void Move(double deltaT);
};

class Enemy1 : public Plane {
public:
    Enemy1(const Vector2 &position, const Vector2 &direction)
         : Plane(position, direction, PLANE_NORMAL) {};  
    void Draw();  
};


typedef DoublyLinkedList<Plane*> PlaneList;
typedef ListNode<Plane*> PlaneNode;


#endif
