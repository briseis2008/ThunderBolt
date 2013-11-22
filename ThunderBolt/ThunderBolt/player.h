#ifndef THUNDERBOLT_PLAYER_H
#define THUNDERBOLT_PLAYER_H

#include "plane.h"


/* class for player: Thunder!!!!!! */
class Thunder : public Plane {
protected:
    /* how many lives do we have now. How I wish it's infinite! */
    int life_num;
    
    /* These are three missile prototype that thunder might be equipped with,
       So that we could easily switch Thunder's weapon between each other */
    Missile *weapon[3];
    
    /* parse user mouse action so that we know whether fire or not */
    void ParseAction(int mouse);
    
public:
    Thunder(const Vector2 &position, const Vector2 &direction);
    virtual ~Thunder();
    
    virtual void Draw();
    virtual void Move(double deltaT);
    virtual void Shoot(int action, MissileList &missiles);
    
    void PowerUp(MissileList &missiles);
    void SwitchWeapon(MissileType type, MissileList &missiles);

};

#endif
