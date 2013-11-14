#ifndef ThunderBolt_Missile_h
#define ThunderBolt_Missile_h

#include <stdio.h>
#include <math.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "vector.h"
#include "color.h"
#include "global.h"
#include "linked_list.h"


enum MissileType
{
    BULLET,
    CANNON,
    LASER,
};

class Missile
{
protected:
    Color color;
    Vector2 position;
    Vector2 velocity;
    Vector2 direction;

    MissileType type;
    int power;

    //TODO:make sure we really need these two parameters
    int state;
    int shootMode;

public:
    Missile();
    Missile(MissileType type, const Color &color, int power, 
            const Vector2 &position, const Vector2 &velocity, int shootMode);
    /*
    Missile(MissileType type, int color, int power, int shootMode, const Vector2 &position);
    Missile(MissileType type, int color, Vector2 position, Vector2 velocity);
    */
    virtual ~Missile() {};

    virtual void Draw(void) = 0;
    virtual int CheckInWindow(void);
    virtual void Move();

    void setType(MissileType type);
    MissileType getType();
    
    void setColor(const Color &color);
    
    void setPosition(const Vector2 &position);
    Vector2 getPosition();
    
    void setVelocity(const Vector2 &velocity);
    void setDirection(const Vector2 &direction);
    
    void setState(int state);
    int getState();
    
    void Launch(Vector2 position);
    void Move(Vector2 newPosition);

};

class Bullet : public Missile {
    double len;
public:
    Bullet(const Color &color, int power, const Vector2 &position, 
           const Vector2 &velocity, int shootMode, double l = 10.0) 
         : Missile(BULLET, color, power, position, velocity, shootMode),
           len(l){};
    void Draw();
};

class Cannon : public Missile {
    double radius;
public:
    Cannon(const Color &color, int power, const Vector2 &position, 
           const Vector2 &velocity, int shootMode, double r = 10.0) 
         : Missile(CANNON, color, power, position, velocity, shootMode), 
           radius(r) {};
    void Draw();
};

class Plane;

class Laser : public Missile {
    int countDown;
    double width;
    /* so that we could keep track of plane direction & position */
    Plane *plane;
public:
    Laser (const Color &color, int power, const Vector2 &position, 
           const Vector2 &velocity, int shootMode, double w = 5.0) 
         : Missile(LASER, color, power, position, velocity, shootMode), 
           countDown(20), width(w) {};
    void Draw();
    int CheckInWindow();
    void setPlane(Plane *plane);
    void Move();
};

/* allocate and create a new missile according to "Missile *missile" */
Missile *CopyMissile(Missile *missile);

typedef DoublyLinkedList<Missile*> MissileList;
typedef ListNode<Missile*> MissileNode;

#endif
