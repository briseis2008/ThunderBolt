#ifndef __ThunderBolt__Plane__
#define __ThunderBolt__Plane__

#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "color.h"
#include "missile.h"

class Plane
{
protected:
    int plane_state;
    int life_num;
    int size_x, size_y;
    Vector2 position;
    Vector2 velocity;
    Vector2 direction;
    int missile_state;
    
public:
    Plane();
    
    Plane(int plane_state, Vector2 position, Vector2 velocity, int missile_state);
    
    void Draw();

    //TODO: Need state?
    int getPlaneState();
    
    void Move();
    
    void Disappear(void);
    
    int CheckHit(Missile *missile);
    
    void Shoot(int key, MissileList &missile);
    
    void setMissile(MissileType type, int color, Vector2 position, Vector2 velocity);
    
};


typedef DoublyLinkedList<Plane *> PlaneList;
typedef ListNode<Plane *> PlaneNode;


#endif