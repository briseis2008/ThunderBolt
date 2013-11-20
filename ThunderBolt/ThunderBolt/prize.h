/*
 Thunderbolt prize.h edition 2
 Created by Yan Zhang
 Andrew ID: yanz3
 */
#ifndef ThunderBolt_Prize_h
#define ThunderBolt_Prize_h

#include <stdio.h>
#include <math.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "vector.h"
#include "color.h"
#include "global.h"
#include "linked_list.h"
#include "missile.h"

enum PrizeType
{
    LIFE
};

class Prize
{
    
protected:
    
    int nprize;
    int p_sizex, p_sizey;
    Vector2 position;
    Vector2 velocity;
    PrizeType type;
    bool c;
    
public:
    int p_x0;
    int prize_lifetime;
    int prize_state;
    Prize();
    void Initialized();
    virtual void Draw();
    virtual void Move();
    //virtual int CheckCatchByPlane(Plane &plane);
    virtual void Disappear(void);
    //virtual void setType(MissileType &type);
    Prize getType() const;

};
#endif
