//
//  Plane.cpp
//  ThunderBolt
//
//  Created by yigu on 10/31/13.
//  Copyright (c) 2013 CMU. All rights reserved.
//

#include "plane.h"
#include "missile.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "debug.h"



/* default constructor */
Plane::Plane()
{
    this->plane_state = PLANE_NORMAL;
    this->size_x=50;
    this->size_y=50;
    this->position = gZero;
    this->direction = gZero;
    this->velocity = 0.0;
    this->missile = NULL;
    this->missile_level = 1;
    this->missile_reload = 0;
    this->laser = NULL;
    this->firing = 0;
    this->life = 0;
    this->default_reload_speed = 10;
}

Plane::Plane(const Vector2 &position, const Vector2 &direction, int plane_state,
             int size_x, int size_y, int life, int default_reload_speed)
{
    this->plane_state = plane_state;
    this->size_x = size_x;
    this->size_y = size_y;
    this->position = position;
    this->direction = direction;
    /* normalize direction vector so that we always have unit vector */
    this->direction.normalize();
    this->velocity = 0.0;
    this->missile = NULL;
    this->missile_level = 1;
    this->missile_reload = 0;
    this->laser = NULL;
    this->firing = 0;
    this->life = life;
    this->default_reload_speed = default_reload_speed;
}

/* we are nice people, so free memory before we go */
Plane::~Plane() {
    if (missile) {
        delete missile;
    }
}

/* getters and setters */
int Plane::getPlaneState() const{
    return plane_state;
}
void Plane::setPlaneState(int state) {
    this->plane_state = state;
}
void Plane::setVelocity(double velocity) {
    this->velocity = velocity;
}
Vector2 Plane::getPosition() const {
    return position;
}


/* Missile* missile member variable in Plane class is used to record what kind
 * of missile will the plane launch while firing.
 * This function is used to modify that variable.
 * @param missile The missile we are going to use (no copy)
 * @param missiles This variable is needed, because: if this function is called
 *                 while the plane is shooting laser, we want to update our
 *                 weapon and delete the current laser. Otherwise, imagine this
 *                 situation: the user is holding mouse button and shooting 
 *                 laser, meanwhile he switches weapon to bullet without loosing
 *                 mouse button, the bullets will be fired correctly however the
 *                 laser will never disappear since no one destroy it
 */
void Plane::setMissile(Missile *missile, MissileList &missiles) {
    /* get the new missile */
    this->missile = missile;

    /* reset missile level, reload time and reload speed */
    missile_level = 1;
    missile_reload = 0;
    reload_speed = default_reload_speed;
    
    /* make sure we cancel current laser and reload */
    ReloadLaser(missiles);   
}

/* Missile* missile member variable in Plane class is used to record what kind
 * of missile will the plane launch while firing.
 * This function is used to modify that variable.
 * @param missiles The missile list, we need this parameter for similar reason.
 *                 See description above for more details.
 */
void Plane::setMissile(MissileType type, const Color &color, int power,
                       const Vector2 &velocity, MissileList &missiles) {
    Missile *newMissile = NULL;
    /* generate one new missile according to type and other parameters */
    switch (type) {
        case BULLET:
            newMissile = new Bullet(color, power, gZero, velocity, 1);
            break;
            
        case CANNON:
            newMissile = new Cannon(color, power, gZero, velocity, 1);
            break;
            
        case LASER:
            newMissile = new Laser(color, power, gZero, velocity, 1);
            break;
    }
    assert(newMissile);
    
    /* if this plane is equiped with a missile before, free it */
    if (this->missile) {
        delete this->missile;
    }
    
    setMissile(newMissile, missiles);

}


/* equip current plane with a missile, this function will make a copy from input
 * @param missile The missile prototype we are going to copy from
 * @param missiles The missile list, we need this parameter for similar reason.
 *                 See description above for more details.
 */
void Plane::setMissileCopy(const Missile *missile, MissileList &missiles) {
    /* copy a missile from prototype, i.e. allocate new memory for missile */
    Missile *newMissile = CopyMissile(missile);
    
    /* free previously equipped missile and equip with new one */
    if (this->missile) {
        delete this->missile;
    }
    
    setMissile(newMissile, missiles); 
}


/* missile initial position is at the front edge of plane */
Vector2 Plane::getMissilePos() {
    return position + direction * (size_y / 2);
}



/* move plane according to current speed */
void Plane::Move(double deltaT){
    /* deltaS = v * deltaT */
    position += (direction * velocity) * deltaT;
}


/* check if plane is hit by a single missile (considering different types) */
int Plane::CheckHit(Missile *missile)
{
    double dist;
    switch (missile->getType()) {
        /* for performance, treat bullet as a single point */
        case BULLET:
            /* distance between circle and point */
            dist = position.distance(missile->position);
            if (dist <= size_x / 2) return 1;
            break;
            
        case CANNON:
            /* distance between two circles */
            dist = position.distance(missile->position);
            if (dist <= size_x / 2 + ((Cannon *)missile)->radius) return 1;            
            break;
            
        /* laser is special, have to calculate distance from plane to laser */
        case LASER:
            double a = missile->direction.y;
            double b = -missile->direction.x;
            double c = missile->direction.x * missile->position.y 
                     - missile->direction.y * missile->position.x;
            /* equation: distance from circle to line */
            dist = fabs(a * position.x + b * position.y + c)
                 / sqrt(a * a + b * b);
            if (dist <= size_x / 2 + ((Laser *)missile)->width) return 1;
            break;
    }
    
    /* if we get here, then no hit is detected */
    return 0;
}

/* check hit with all missiles in a missile list (linked list), 
 * return 1 if finally destroyed by missiles; 0 otherwise
 */
int Plane::CheckHit(MissileList &missiles) {
    /* get the head of list */
    MissileNode *node = missiles.getFront();
    
    /* traverse the missiles in the list and check for hit */
    while (node) {
        /* typeof(node->dat) is Missile*  */
        if (CheckHit(node->dat)) {
            /* if hit by a missile, we get hurt :( */
            life -= node->dat->power;
            
            /* the missile will disappear after collision, unless it's laser */
            if (node->dat->getType() != LASER) 
                node = missiles.Delete(node);
            else 
                node = node->next;
            
            /* check for early termination, return 1 if destroyed */
            if (life <= 0) return 1;
        } else {
            node = node->next;            
        }
    }
    
    /* if we get here, then plane is not destroyed */
    return 0;
}

/* check hit with another plane */
int Plane::CheckHit(Plane *plane) {
    double dist;
    /* basic circle to circle collision detection */
    dist = position.distance(plane->position);
    if (dist <= size_x / 2 + plane->size_x / 2) return 1;
    
    return 0;
}


/* Cancel current laser emitting, delete it from missile list */
void Plane::ReloadLaser(MissileList &missiles) {
    /* only do this while we are actually firing laser */
    if (laser) {
        /* delete it from missile list */
        missiles.Delete(laser);
        laser = NULL;
        /* get ready for next fire */
        missile_reload = 0;
    }
}


/**
 * @brief perform shooting action according to user input
 * 
 * 1) if the plane is firing: we fire a missile according to the missile we are
 *    equipped with. We make one new missile, set up position and direction,
 *    then insert it into missile linked list. If we are firing bullet or cannon
 *    , plane will have no control over that missile later on, and the missile 
 *    will move by itself; if we are emitting laser, we need to keep track of
 *    that laser beam so that we could easily cancel it if needed.
 * 2) if the plane stops firing: we need to cancel current laser if we are 
 *    actually firing one
 *
 * @param action firing if this input is 1. not firing otherwise.
 * @param missiles the missile list we are going to add our missile in
 */
void Plane::Shoot(int action, MissileList &missiles){
    firing = action;
    
    if (firing == 1) {
        /* wait for weapon to cool down */
        if (missile_reload > 0) return;

        Vector2 shootPosition = getMissilePos();
    
        /* create a new missile according to our armory */
        Missile *newMissile = CopyMissile(missile);
        newMissile->setPosition(shootPosition);
        
        /* missile gets the same direction as plane */
        newMissile->setDirection(direction);
        
        /* add this new fired missile into linked list */
        MissileNode *newNode = missiles.InsertBack(newMissile);
        /* right after shoot, refresh our reload time */
        switch(newMissile->getType()) {
            case BULLET:
                missile_reload = 100;
                break;
                
            case CANNON:
                missile_reload = 300;
                break;
                
            case LASER:
                /* in case it's laser, we also need to record this missile node
                   so that we could cancel it by ourselves in future */
                laser = newNode;
                /* also record this plane in that laser. Because Laser->Move()
                   will actually query current plane poisition & direction */
                ((Laser *)newMissile)->setPlane(this);
                missile_reload = 1;
                break;
        }
        
        // FIXME!!!
        /* The following part is a temporary work-around to get multi-direction
           bullets. It's here for demo purpose ONLY. We should come up with
           some structured code that emits bullets according to current
           missile_level */
        /*********** ATTENTION: Below is a temporary hack ONLY! ***************
        
        {
            if (newMissile->getType() == BULLET) {
                newMissile = CopyMissile(missile);
                newMissile->setPosition(shootPosition+ Vector2(5, 0));
                newMissile->setDirection(direction);
                missiles.InsertBack(newMissile);
    
                newMissile = CopyMissile(missile);
                newMissile->setPosition(shootPosition+ Vector2(-5, 0));
                newMissile->setDirection(direction);
                missiles.InsertBack(newMissile);
    
                newMissile = CopyMissile(missile);
                newMissile->setPosition(shootPosition+ Vector2(10, 0));
                newMissile->setDirection(direction);
                missiles.InsertBack(newMissile);
    
                newMissile = CopyMissile(missile);
                newMissile->setPosition(shootPosition+ Vector2(-10, 0));
                newMissile->setDirection(direction);
                missiles.InsertBack(newMissile);
    
                Vector2 newDirection = direction;
                newDirection.rotate(0.5);
                
                newMissile = CopyMissile(missile);
                newMissile->setPosition(shootPosition+ Vector2(5, 5));
                newMissile->setDirection(newDirection);
                missiles.InsertBack(newMissile);
    
                newMissile = CopyMissile(missile);
                newMissile->setPosition(shootPosition+ Vector2(10, 10));
                newMissile->setDirection(newDirection);
                missiles.InsertBack(newMissile);
    
                newMissile = CopyMissile(missile);
                newMissile->setPosition(shootPosition+ Vector2(15, 15));
                newMissile->setDirection(newDirection);
                missiles.InsertBack(newMissile);
    
                newMissile = CopyMissile(missile);
                newMissile->setPosition(shootPosition+ Vector2(20, 20));
                newMissile->setDirection(newDirection);
                missiles.InsertBack(newMissile);
    
                newMissile = CopyMissile(missile);
                newMissile->setPosition(shootPosition+ Vector2(25, 25));
                newMissile->setDirection(newDirection);
                missiles.InsertBack(newMissile);
 
                newDirection.rotate(-1);
                newMissile = CopyMissile(missile);
                newMissile->setPosition(shootPosition+ Vector2(-5, 5));
                newMissile->setDirection(newDirection);
                missiles.InsertBack(newMissile);
    
                newMissile = CopyMissile(missile);
                newMissile->setPosition(shootPosition+ Vector2(-10, 10));
                newMissile->setDirection(newDirection);
                missiles.InsertBack(newMissile);
    
                newMissile = CopyMissile(missile);
                newMissile->setPosition(shootPosition+ Vector2(-15, 15));
                newMissile->setDirection(newDirection);
                missiles.InsertBack(newMissile);
    
                newMissile = CopyMissile(missile);
                newMissile->setPosition(shootPosition+ Vector2(-20, 20));
                newMissile->setDirection(newDirection);
                missiles.InsertBack(newMissile);
    
                newMissile = CopyMissile(missile);
                newMissile->setPosition(shootPosition+ Vector2(-25, 25));
                newMissile->setDirection(newDirection);
                missiles.InsertBack(newMissile);
    
            }
        }
        *********** ATTENTION: Above is a temporary hack ONLY! ***************/
        
    } else {
        ReloadLaser(missiles);
    }


    
}


/* Cool down the weapon, decrease the reload time count down. Such that, when
 * realod == 0, we are ready for firing next missile. This function should be
 * called in each game iteration.
 * reload_speed will affect how fast the reloading process is.
 */
void Plane::CoolDown(){
    /* the weapon is already reloaded */
    if (missile_reload <= 0) return;
    /* laser is firing with out interval */
    if (missile->getType() == LASER) return;
    
    /* reload could be control */
    missile_reload -= reload_speed;
}


/* check if the plane is still in window */
int Plane::CheckInWindow() {
    if (position.x >= 0 && position.x < WINDOW_WID
     && position.y >= 0 && position.y < WINDOW_HEI)
        return 1;
        
    return 0;
}

/* change direction so that we are aiming the target plane */
void Plane::Aim(Plane *target) {
    direction = target->getPosition() - position;
    direction.normalize();
}


#ifdef PLANE_DEBUG
int main(void){
    Vector2 startPosition(300, 280);
    Vector2 startDirection(0, -1);
    
    /* This list is used to store all user emmitted active missiles */
    MissileList missiles;
    /* This list is used to store all moving enemies */
    PlaneList enemies;
    
    /* Create the thunder */
    Thunder thunder(startPosition, startDirection);
    thunder.setVelocity(5);
    thunder.SwitchWeapon(BULLET, missiles);

    Plane *player;
    player = &thunder;
    
    /* create the stupid enemy who stands there forever. This is for testing
       purpose and this enemy will check hit with bullets however never be
       destroyed 
       Yeah Baby, I am The White Walker!!! */
    Enemy1 enemy1(Vector2(500, 100), Vector2(0,1));
    Plane *enemy;
    enemy = &enemy1;
    
    /* testing boss */
    Boss theBoss(Vector2(300, 100), Vector2(0,1));
    Plane *boss;
    boss = &theBoss;
    
    FsOpenWindow(0,0,WINDOW_WID,WINDOW_HEI,1);
    glClearColor(0.1, 0.1, 0.1, 1);
    
    bool running = true;
    int i = 0;
    int j = 0;
    
    while(running)
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        FsPollDevice();
        
        int key=FsInkey();
        int lb, mb, rb, mx, my;
        int mouse = FsGetMouseEvent(lb, mb, rb, mx, my);

        /* generate a moving enemy once in a while, it starts from top left
           corner, and is moving towards right-bottom */
        if (j % 100 == 0) {
            Plane *newP = new Enemy1(Vector2(0, 0), Vector2(1, 1));
            newP->setVelocity(2.0);
            enemies.InsertBack(newP);
        }
        j++;
        
        if(FSKEY_ESC==key)
        {
            running=false;
            break;
        } else if (FSKEY_UP == key) {
            /* UP for power up */
            ((Thunder *)player)->PowerUp(missiles);
        }
        if (FSMOUSEEVENT_RBUTTONDOWN == mouse) {
            /* Right click for switch between 3 types of weapons */
            i = (i+1) % 3;
            ((Thunder *)player)->SwitchWeapon((MissileType)i, missiles);
        }
        
        /* Thunder: shoot and cool down weapon. */
        player->Shoot(mouse, missiles);
        player->CoolDown();
        
        /* Thunder: move and draw. */
        player->Move(1.0);
        player->Draw();
        
        /* Draw that Immortal guy */
        enemy->Aim(player);
        enemy->Draw();

        /* Draw the boss */
        boss->Aim(player);
        boss->Draw();        
        
        /* traverse the missiles list, move missile, and check hit with that 
           immortal enemy, just for test CheckHit function */
        MissileNode *node;
        node = missiles.getFront();
        while(node) {
            node->dat->Move(1.0);
            
            if (!node->dat->CheckInWindow()) {
                node = missiles.Delete(node);
            } else if (enemy->CheckHit(node->dat)) {
                printf("Hit by missile!\t%p\t%p\n", node, node->dat);
                /* only delete when not laser */
                if (node->dat->getType() != LASER) node = missiles.Delete(node);
                else node = node->next;
            } else {
                node = node->next;
            }
        }

        /* traverse the moving enemy list, check hit with all missiles for each
           enemy, delete it if destroyed */
        PlaneNode *pNode;
        pNode = enemies.getFront();
        while(pNode) {
            pNode->dat->Move(1.0);
            pNode->dat->Aim(player);
            
            if (!pNode->dat->CheckInWindow()) {
                pNode = enemies.Delete(pNode);
            } else {
                if (pNode->dat->CheckHit(missiles))
                    pNode = enemies.Delete(pNode);
                else
                    pNode = pNode->next;
            }
        }

        /* traverse enemies and missiles, draw them */
        FOR_EACH(pNode, enemies) {
            pNode->dat->Draw();
        }
        FOR_EACH(node, missiles) {
            node->dat->Draw();
        }
        
        
        FsSwapBuffers();
        FsSleep(25);
    }
    
    return 0;
}
#endif



