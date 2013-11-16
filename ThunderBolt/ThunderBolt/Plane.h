#ifndef __ThunderBolt__Plane__
#define __ThunderBolt__Plane__

#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "missile.h"


#define PLANE_NORMAL 1      // normal state
#define PLANE_SUPER  2      // super state, can not be demaged

#define MAX_MISSILE_LEVEL 5 // maximum weapon level

/* abstract class for all moving objects */
class Plane
{
protected:
    /* basic properties */
    Vector2 position;
    Vector2 direction;  // should be unit vector
    double velocity;    // velocity magnitude. So v = direction*velocity
    
    /* used to record what kind of missile will the plane launch while firing,
       can be modified using setMissile() */
    Missile *missile;
    /* current weapon level, for possible weapon upgrade */
    int missile_level;
    /* control interval between missile shooting, shoot is allowed ONLY when
       this value is zero */
    int missile_reload;
    /* control reload speed, the higher the faster reloading is */
    int reload_speed;
    /* keep track of this, so that we could switch back to default reload
       speed after switch between different types of missiles */
    int default_reload_speed;
    
    /* keep track of the laser we shoot so we could stop it when needed */
    MissileNode *laser;
    
    /* whether we are shooting */
    int firing;
    
    int size_x, size_y;    
    int plane_state;
    /* life value. NOTE: different from life_num which is number of lives */
    int life;
    
    void ParseAction(int mouse);
    void ReloadLaser(MissileList &missiles);
    Vector2 getMissilePos();
    
public:
    Plane();    
    Plane(const Vector2 &position, const Vector2 &direction, int plane_state, 
          int size_x = 50, int size_y = 50, int life = 100, 
          int default_reload_speed = 10);
    virtual ~Plane();
    
    int getPlaneState();
    void setPlaneState(int state);
    void setVelocity(double velocity);
    
    virtual void Draw() = 0;
    virtual void Move(double deltaT);
    virtual int CheckHit(Missile *missile);
    virtual int CheckHit(MissileList &missiles);
    virtual int CheckHit(Plane *plane);

    virtual void Shoot(int action, MissileList &missiles);
    virtual void CoolDown();
    
    void setMissile(MissileType type, const Color &color, int power,
                    const Vector2 &velocity, MissileList &missiles);
    void setMissile(const Missile *missile, MissileList &missiles);
    
    int CheckInWindow();
    void PowerUp(MissileList &missiles);
    
    friend class Laser;
};

/* class for player: Thunder!!!!!! */
class Thunder : public Plane {
protected:
    /* how many lives do we have now. How I wish it's infinite! */
    int life_num;
    
public:
    Thunder(const Vector2 &position, const Vector2 &direction);
    void Draw();
    void Move(double deltaT);
};

/* Some dummy stupid enemy who only knows how to move slowly */
class Enemy1 : public Plane {
public:
    Enemy1(const Vector2 &position, const Vector2 &direction)
         : Plane(position, direction, PLANE_NORMAL, 50, 50, 1000) {};  
    void Draw();
};


typedef DoublyLinkedList<Plane*> PlaneList;
typedef ListNode<Plane*> PlaneNode;


#endif
