#include "LazerEnemy.h"
#include "plane.h"
#include "missile.h"
#include "player.h"
#include "enemy.h"
#include "debug.h"
#include "ysglfontdata.h"

void LazerArm::Init(MissileList &missiles) {
    Missile *weapon = new Laser(gBlue, 20, gZero, Vector2(0, -1), 1);
    setMissile(weapon, missiles);
}

void LazerArm::ReloadLaser(MissileList &missiles) {
    this->Plane::ReloadLaser(missiles);
}

void LazerEnemy::Init(MissileList &missiles) {
    arm.Init(missiles);
}

// draw the enemy
void LazerEnemy::Draw() {
    if (!alive)
        return;
    
    glColor3ub(100, 100, 100);
    
    glBegin(GL_QUADS);
    
    glVertex2i(position.x-5,position.y+20);
    glVertex2i(position.x+4,position.y+20);
    glVertex2i(position.x+4,position.y+30);
    glVertex2i(position.x-5,position.y+30);
    
    glVertex2i(position.x-15,position.y+40);
    glVertex2i(position.x+14,position.y+40);
    glVertex2i(position.x+14,position.y+30);
    glVertex2i(position.x-15,position.y+30);
    glVertex2d(position.x,position.y);
    
    glEnd();
    
    arm.Draw();
}

void LazerEnemy::Shoot(MissileList &missiles) {
    if (!alive)
        return;
    
    arm.Shoot(1, missiles);
}

void LazerEnemy::CoolDown() {
    if (!alive)
        return;
    arm.CoolDown();
}

void LazerEnemy::Move(double deltaT) {
    if (!alive)
        return;
    
    position += (direction * velocity) * deltaT;
    
    arm.SetPosition(Vector2(0, 60) + position);
    
    printf("\n%d\n", wander);
    /* Move down */
    if (position.y > WINDOW_HEI/3 && !wander) {
        printf("\n\nin the loop\n\n\n");
        wander = true;
        printf("%d", wander);
        direction.set(-1.0, 0.0);
    }
    
    /* Wandering at 1/3 window height */
    if (wander) {
        if(position.x<=100 && direction.x<0)
            printf("\n\nx-axis%f\n", position.x);
			direction.x = -direction.x;
		if(position.x>=WINDOW_WID-100 && direction.x>0)
            printf("\n\nx-axis%f\n", position.x);
			direction.x = -direction.x;
    }
    
    
    
    
    
}

/* check hit with all missiles in a missile list (linked list),
 * return 1 if finally destroyed by missiles; 0 otherwise
 */
int LazerEnemy::CheckHit(MissileList &missiles) {
    if (!alive)
        return -1;
    /* get the head of list */
    MissileNode *node = missiles.getFront();
    
    /* traverse the missiles in the list and check for hit */
    while (node) {
        /* typeof(node->dat) is Missile*  */
        if (this->Plane::CheckHit(node->dat) ||
            arm.CheckHit(node->dat)) {
            /* if hit by a missile, the enemy get hurt :( */
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
    
    /* if we get here, then enemy is not destroyed */
    return 0;
}

int LazerEnemy::getLife() const {
    return life;
}

void LazerEnemy::Disappear(MissileList &missiles) {
    alive = false;
    arm.ReloadLaser(missiles);
}

void PrintPower(int power)
{
    char str[256];
    if (power > 0)
        sprintf (str, "Remaining Power: %d", power);
    else
        sprintf (str, "Lazer Enemy is Died!");
    glColor3ub(255,255,255);
    glRasterPos2d(100,700);
    YsGlDrawFontBitmap8x12(str);
}


#ifdef LAZERENEMY_DEBUG
int main(){
    
    Vector2 startPosition(300, 500);
    Vector2 startDirection(0, -1);
    
    /* This list is used to store all user emmitted active missiles */
    MissileList enemy2Missiles;
    MissileList playerMissiles;
    
    /* Create the thunder */
    Thunder thunder(startPosition, startDirection);
    thunder.setVelocity(5);
    thunder.SwitchWeapon(BULLET, playerMissiles);
    
    Plane *player;
    player = &thunder;
    
    /* Create the boss */
    LazerEnemy enemy2(Vector2(300, 0), Vector2(0,1));
    enemy2.Init(enemy2Missiles);
    enemy2.setVelocity(15);
    Plane *enemy;
    enemy = &enemy2;
  
    
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
        
        /* Draw the enemy */
        enemy->Aim(player);
        printf("before move\n");
        enemy->Move(0.4);
        printf("after move\n");
        enemy->Draw();
        /* Enemy fire */
        ((LazerEnemy *)enemy)->Shoot(enemy2Missiles);
        enemy->CoolDown();
        
        if (enemy->CheckHit(playerMissiles) == 1) {
            ((LazerEnemy *)enemy)->Disappear(enemy2Missiles);
            cntDown = 100;
        }
        
        /* Stay for a while after boss die */
        if (cntDown > 0) {
            cntDown--;
            if (cntDown == 0)
                running = false;
        }
        
        /* traverse the missiles list, move missile */
        MissileNode *node;
        node = enemy2Missiles.getFront();
        while(node) {
            node->dat->Move(1.0);
            
            if (!node->dat->CheckInWindow()) {
                node = enemy2Missiles.Delete(node);
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
        
        FOR_EACH(node, enemy2Missiles) {
            node->dat->Draw();
        }
        FOR_EACH(node, playerMissiles) {
            node->dat->Draw();
        }
        PrintPower(((LazerEnemy *)enemy)->getLife());
        FsSwapBuffers();
        FsSleep(25);
    }
    
    return 0;
}

#endif


