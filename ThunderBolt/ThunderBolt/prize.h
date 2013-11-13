#ifndef ThunderBolt_Prize_h
#define ThunderBolt_Prize_h

#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "data.h"




class Prize
{
    
private:
    
    //PrizeType type;      // ?type powerup? bulletip?
    
    
    int nprize;
    
    int p_sizex, p_sizey;
    Vector position;
    Vector velocity;
    
public:
    int p_x0;
    int prize_lifetime;
    int prize_state;
    Prize();
    void Initialized();
    void Draw();
    void Move();
    //int Checkcatch(Plane plane);
    void Disappear(void);

    

};
#endif
