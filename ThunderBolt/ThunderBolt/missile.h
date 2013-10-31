#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"

#ifndef ThunderBolt_Missile_h
#define ThunderBolt_Missile_h


const double YS_PI=3.1415927;

enum MissileType
{
    BULLET,
    CANNON,
    LASER,
};

class Missile
{
private:
    MissileType type;
    int state;
    int color;
    int red;
    int green;
    int blue;
    int power;
    int shootMode;
    int countDown;
    Vector position;
    Vector velocity;
public:
    Missile(MissileType type, int color, int power, int shootMode, Vector position, Vector velocity);
    Missile(MissileType type, int color, int power, int shootMode, Vector position);
    Missile(MissileType type, int color, Vector position);
    Missile();
    void Launch(Vector position);
    void Draw(void);
    void Move(Vector newPosition);
    void Move();
    int CheckInWindow(void);
    Vector getPosition();
};

#endif
