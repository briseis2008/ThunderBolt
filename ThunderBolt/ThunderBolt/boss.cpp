#include "boss.h"
#include "plane.h"
#include "missile.h"
#include "player.h"
#include "enemy.h"
#include "debug.h"
#include "ysglfontdata.h"


void Arm::Draw() {
    glColor3ub(color.r(), color.g(), color.b());
    
    glBegin(GL_POLYGON);
    glVertex2d(position.x + direction.x * size_y/2,
               position.y + direction.y * size_y/2);
    glVertex2d(position.x - direction.x * size_y/2 + direction.y * size_x/2,
               position.y - direction.y * size_y/2 - direction.x * size_x/2);
    glVertex2d(position.x - direction.x * size_y/2 - direction.y * size_x/2,
               position.y - direction.y * size_y/2 + direction.x * size_x/2);
    glEnd();
}

void Arm::Init(MissileList &missiles) { }

void Arm::SetPosition(const Vector2 &newPosition) {
    position = newPosition;
}

void BossLeftArm::Init(MissileList &missiles) {
    Missile *weapon = new Bullet(gPurple, 300, gZero, Vector2(0, -20), 1);
    setMissile(weapon, missiles);
}

void BossRightArm::Init(MissileList &missiles) {
    Missile *weapon = new Cannon(gYellow, 1000, gZero, Vector2(0, -10), 1);
    setMissile(weapon, missiles);
}

void BossMidArm::Init(MissileList &missiles) {
    Missile *weapon = new Laser(gBlue, 20, gZero, Vector2(0, -1), 1);
    setMissile(weapon, missiles);
}

void BossMidArm::ReloadLaser(MissileList &missiles) {
    this->Plane::ReloadLaser(missiles);
}

void Boss::Init(MissileList &missiles) {
    leftArm.Init(missiles);
    midArm.Init(missiles);
    rightArm.Init(missiles);
}

void Boss::Draw() {
    if (!alive)
        return;
    
    glColor3ub(100, 100, 100);
    
    glBegin(GL_POLYGON);
    int i;
    for(i=0; i<64; i++)
    {
        double angle=(double)i*PI/32.0;
        double x=position.x + cos(angle)*size_x / 2;
        double y=position.y + sin(angle)*size_x / 2;
        glVertex2d(x,y);
    }

    glEnd();
    
    leftArm.Draw();
    midArm.Draw();
    rightArm.Draw();
}

void Boss::Aim(Plane *target) {
    if (!alive)
        return;
    leftArm.Aim(target);
    rightArm.Aim(target);
    //midArm.Aim(target);
}

void Boss::Shoot(MissileList &missiles) {
    if (!alive)
        return;
    leftArm.Shoot(1, missiles);
    rightArm.Shoot(1, missiles);
    midArm.Shoot(1, missiles);
}

void Boss::CoolDown() {
    if (!alive)
        return;
    leftArm.CoolDown();
    rightArm.CoolDown();
    midArm.CoolDown();
}

void Boss::Move(double deltaT) {
    if (!alive)
        return;
    position += (direction * velocity) * deltaT;
    leftArm.SetPosition(Vector2(-75, 75) + position);
    rightArm.SetPosition(Vector2(75, 75) + position);
    midArm.SetPosition(Vector2(0, 75) + position);
    
    /* Move down */
    if (position.y > WINDOW_HEI/3 && !wander) {
        wander = true;
        direction.set(-1.0, 0.0);
    }
    
    /* Wandering at 1/3 window height */
    if (wander) {
        if(position.x<=100 && direction.x<0)
			direction.x = -direction.x;
		if(position.x>=WINDOW_WID-100 && direction.x>0)
			direction.x = -direction.x;
    }
}

/* check hit with all missiles in a missile list (linked list),
 * return 1 if finally destroyed by missiles; 0 otherwise
 */
int Boss::CheckHit(MissileList &missiles) {
    if (!alive)
        return -1;
    /* get the head of list */
    MissileNode *node = missiles.getFront();
    
    /* traverse the missiles in the list and check for hit */
    while (node) {
        /* typeof(node->dat) is Missile*  */
        if (this->Plane::CheckHit(node->dat) ||
            leftArm.CheckHit(node->dat) ||
            rightArm.CheckHit(node->dat) ||
            midArm.CheckHit(node->dat)) {
            /* if hit by a missile, we get hurt :( */
            //TODO: life minus twice
            life -= node->dat->getPower();
            
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

int Boss::getLife() const {
    return life;
}

void Boss::Disappear(MissileList &missiles) {
    alive = false;
    midArm.ReloadLaser(missiles);
}

void PrintPower(int power)
{
    char str[256];
    if (power > 0)
        sprintf (str, "Remaining Power: %d", power);
    else
        sprintf (str, "Boss is Died!");
    glColor3ub(255,255,255);
    glRasterPos2d(100,700);
    YsGlDrawFontBitmap8x12(str);
}

#ifdef BOSS_DEBUG
int main(void){
    Vector2 startPosition(300, 500);
    Vector2 startDirection(0, -1);
    
    /* This list is used to store all user emmitted active missiles */
    MissileList bossMissiles;
    MissileList playerMissiles;
    
    /* Create the thunder */
    Thunder thunder(startPosition, startDirection);
    thunder.setVelocity(5);
    thunder.SwitchWeapon(BULLET, playerMissiles);
    
    Plane *player;
    player = &thunder;
    
    /* Create the boss */
    Boss theBoss(Vector2(300, 100), Vector2(0,1));
    theBoss.Init(bossMissiles);
    theBoss.setVelocity(5);
    Plane *boss;
    boss = &theBoss;
    
    FsOpenWindow(0,0,WINDOW_WID,WINDOW_HEI,1);
    glClearColor(0.1, 0.1, 0.1, 1);
    
    bool running = true;
    int i = 0;
    int cntDown = 0;
    
    while(running)
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        FsPollDevice();
        
        int key=FsInkey();
        int lb, mb, rb, mx, my;
        int mouse = FsGetMouseEvent(lb, mb, rb, mx, my);
        
        if(FSKEY_ESC==key) {
            running=false;
            break;
        } else if (FSKEY_UP == key) {
            /* UP for power up */
            ((Thunder *)player)->PowerUp(playerMissiles);
        }
        if (FSMOUSEEVENT_RBUTTONDOWN == mouse) {
            /* Right click for switch between 3 types of weapons */
            i = (i+1) % 3;
            ((Thunder *)player)->SwitchWeapon((MissileType)i, playerMissiles);
        }
        
        /* Thunder: shoot and cool down weapon. */
        player->Shoot(mouse, playerMissiles);
        player->CoolDown();
        
        /* Thunder: move and draw. */
        player->Move(1.0);
        player->Draw();
        
        /* Draw the boss */
        boss->Aim(player);
        boss->Move(0.4);
        boss->Draw();
        /* Boss fire */
        ((Boss *)boss)->Shoot(bossMissiles);
        boss->CoolDown();
        
        if (boss->CheckHit(playerMissiles) == 1) {
            ((Boss *)boss)->Disappear(bossMissiles);
            cntDown = 200;
        }
        
        /* Stay for a while after boss die */
        if (cntDown > 0) {
            cntDown--;
            if (cntDown == 0)
                running = false;
        }
        
        /* traverse the missiles list, move missile */
        MissileNode *node;
        node = bossMissiles.getFront();
        while(node) {
            node->dat->Move(1.0);
            
            if (!node->dat->CheckInWindow()) {
                node = bossMissiles.Delete(node);
            } else {
                node = node->next;
            }
        }
        node = playerMissiles.getFront();
        while(node) {
            node->dat->Move(1.0);
            
            if (!node->dat->CheckInWindow()) {
                node = playerMissiles.Delete(node);
            } else {
                node = node->next;
            }
        }
        
        FOR_EACH(node, bossMissiles) {
            node->dat->Draw();
        }
        FOR_EACH(node, playerMissiles) {
            node->dat->Draw();
        }
        PrintPower(((Boss *)boss)->getLife());
        FsSwapBuffers();
        FsSleep(25);
    }
    
    return 0;
}

#endif


