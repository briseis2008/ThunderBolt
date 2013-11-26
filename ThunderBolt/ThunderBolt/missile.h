#ifndef ThunderBolt_Missile_h
#define ThunderBolt_Missile_h

#include <stdio.h>
#include <math.h>
#include <time.h>
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

class Plane;

class Missile
{
protected:
    Color color;
    Vector2 position;
    Vector2 velocity;
    Vector2 direction;

    MissileType type;
    /* how much damage can this missile make if hit */
    int power;

    //TODO:make sure we really need these two parameters ?
    int state;
    int shootMode;

public:
    Missile();
    Missile(MissileType type, const Color &color, int power, 
            const Vector2 &position, const Vector2 &velocity, int shootMode);
    virtual ~Missile() {};

    void setType(MissileType type);
    MissileType getType() const;
    
    void setColor(const Color &color);
    
    void setPosition(const Vector2 &position);
    Vector2 getPosition() const;
    
    void setVelocity(const Vector2 &velocity);
    void setDirection(const Vector2 &direction);
    
    void setState(int state);
    int getState() const;
    
    void setPower(int power);
    int getPower() const;

    virtual void Draw(void) = 0;
    virtual int CheckInWindow(void);
    virtual void Move(double deltaT);
        
    friend class Plane;
};

class Bullet : public Missile {
public:
    double len;

    Bullet(const Color &color, int power, const Vector2 &position, 
           const Vector2 &velocity, int shootMode, double l = 4.0) 
         : Missile(BULLET, color, power, position, velocity, shootMode),
           len(l){};
    void Draw();
};

class Cannon : public Missile {
public:
    double radius;

    Cannon(const Color &color, int power, const Vector2 &position, 
           const Vector2 &velocity, int shootMode, double r = 10.0) 
         : Missile(CANNON, color, power, position, velocity, shootMode), 
           radius(r) {};
    void Draw();
};


class Laser : public Missile {
    /* so that we could keep track of plane direction & position */
    Plane *plane;
public:
    double width;

    Laser (const Color &color, int power, const Vector2 &position, 
           const Vector2 &velocity, int shootMode, double w = 5.0) 
         : Missile(LASER, color, power, position, velocity, shootMode), width(w) {};
    void Draw();
    int CheckInWindow();
    void setPlane(Plane *plane);
    void Move(double deltaT);
};

/* allocate and create a new missile according to "Missile *missile" */
Missile *CopyMissile(const Missile *missile);

typedef DoublyLinkedList<Missile*> MissileList;
typedef ListNode<Missile*> MissileNode;

#endif
