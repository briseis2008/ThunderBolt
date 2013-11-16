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
    int size_x, size_y;    
    int life;
    
    void ParseAction(int mouse);
    void ReloadLaser(MissileList &missiles);
    Vector2 getMissilePos();
    
public:
    Plane();    
    Plane(const Vector2 &position, const Vector2 &direction, int plane_state, int life = 100);
    virtual ~Plane();
    
    int getPlaneState();
    void setPlaneState(int state);
    void setVelocity(double velocity);
    
    virtual void Draw() = 0;
    virtual void Move(double deltaT);
    virtual int CheckHit(Missile *missile);
    virtual int CheckHit(MissileList &missiles);
    virtual int CheckHit(Plane *plane);

    void Shoot(int action, MissileList &missiles);
    void CoolDown();
    
    void setMissile(MissileType type, const Color &color, int power,
                    const Vector2 &velocity, MissileList &missiles);
    void setMissile(Missile *missile, MissileList &missiles);
    
    int CheckInWindow();
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
         : Plane(position, direction, PLANE_NORMAL, 1000) {};  
    void Draw();  
};


typedef DoublyLinkedList<Plane*> PlaneList;
typedef ListNode<Plane*> PlaneNode;


#endif
