#include "player.h"


/* constructor for Thunder class */
Thunder::Thunder(const Vector2 &position, const Vector2 &direction) 
               : Plane(position, direction, PLANE_NORMAL, 50, 50, 1000, 15) {
    life_num = 3;
    missileType = 0;
    /* three thunder weapon prototypes */
    weapon[0] = new Bullet(gRed, 300, gZero, Vector2(0, -20), 1);
    weapon[1] = new Cannon(gGreen, 1000, gZero, Vector2(0, -10), 1);
    weapon[2] = new Laser(gBlue, 20, gZero, Vector2(0, -1), 1);
    for (int i = 0; i < 3; i++) assert(weapon[i]);
                   
}

Thunder::~Thunder () {
    for (int i = 0; i < 3; i++) delete weapon[i];
    /* since missile is just pointing to one of the weapon[], we clear it now 
       so that missile will not get deleted again in Plane::~Plane(). */
    missile = NULL;
}


/* Thunder movement simulation. Currently it's not driven by Newton Equation.
   Whenever user press control keys, we move the plane by one step. */
void Thunder::Move(double deltaT) {
    if(FsGetKeyState(FSKEY_A)!=0)
	{
		position.x -= velocity;
	}
	if(FsGetKeyState(FSKEY_D)!=0)
	{
		position.x += velocity;
	}
    if(FsGetKeyState(FSKEY_S)!=0)
    {
        position.y += velocity;
    }
    if(FsGetKeyState(FSKEY_W)!=0){
        position.y -= velocity;
    }
    
    /* you wanna move thunder out of screen? NO WAY! Muahahahaha */
    if (position.x < 0) position.x = 0;
    if (position.x > WINDOW_WID) position.x = WINDOW_WID;
    if (position.y < size_y / 2) position.y = size_y / 2;
    if (position.y > WINDOW_HEI - size_y / 2) position.y = WINDOW_HEI - size_y / 2;
}

void Thunder::upgrade(int upLevel, MissileList &missiles) {
    this->missileType = (this->missileType + upLevel) % 3;
    this->SwitchWeapon(MissileType(this->missileType), missiles);
}


/* Draw the thunder plane */
void Thunder::Draw(){
    glColor3ubv(gRed.arr());
    glBegin(GL_POLYGON);
    glVertex2d(position.x,position.y - size_y/2);
    glVertex2d(position.x + size_x / 2, position.y + size_y/2);
    glVertex2d(position.x - size_x / 2, position.y + size_y/2);
    glEnd();
    
}


/* parse user mouse action, decide whether to fire */
void Thunder::ParseAction(int mouse) {
    if (FSMOUSEEVENT_LBUTTONDOWN == mouse) {
        firing = 1;
    } else if (FSMOUSEEVENT_LBUTTONUP == mouse) {
        firing = 0;
    }
}

/* whethter Thunder fires or not depends on user input, we parse that, then just
 * perform parent class shoot operation */
void Thunder::Shoot(int action, MissileList &missiles) {
    ParseAction(action);
    Plane::Shoot(firing, missiles);
}


/* upgrade the weapon by one level if possible */
void Thunder::PowerUp(MissileList &missiles) {
    if (missile_level < MAX_MISSILE_LEVEL) {
        missile_level++;
        switch (missile->getType()) {
            /* upgrade for bullet: no actual change to missile itself? */
            case BULLET:
                
                break;
                
            /* upgrade for cannon: faster speed? */
            case CANNON:
                reload_speed += 5;
                break;
                
            /* upgrade for laser: wider range? */
            case LASER:
                ReloadLaser(missiles);
                ((Laser *)missile)->width += 2;
        }
    }
}

/* switch current weapon to specified type (bullet, cannon or laser) */
void Thunder::SwitchWeapon(MissileType type, MissileList &missiles) {
    setMissile(weapon[(int)type], missiles);
}
