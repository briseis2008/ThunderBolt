#include "missile.h"
#include "plane.h"
#include "debug.h"

Missile::Missile(MissileType type, const Color &color, int power,
                 const Vector2 &position, const Vector2 &velocity, int shootMode)
{
    this->type = type;
    this->color = color;
    this->power = power;
    this->position = position;
    this->velocity = velocity;
    this->direction = velocity;
    this->direction.normalize();

    state = 1;
    this->shootMode = shootMode;
}

Missile::Missile()
{
    this->color = gBlack;
    this->power = 0;
    this->position = Vector2(0,0);
    this->velocity = Vector2(0,0);
    this->direction = velocity;
    this->direction.normalize();

    state = 0;
    this->shootMode = 0;
}

/*
Missile::Missile(MissileType type, int color, int power, int shootMode, Vector2 position)
{
    state = 0;
    this->type = type;
    this->color = color;
    this->red = (color>>16)&0xff;
    this->green = (color>>8)&0xff;
    this->blue = color&0xff;
    this->power = power;
    this->shootMode = shootMode;
    this->position = position;
   
    
}

Missile::Missile(MissileType type, int color, Vector2 position, Vector2 velocity)
{
    state = 0;
    this->type = type;
    this->color = color;
    this->red = (color>>16)&0xff;
    this->green = (color>>8)&0xff;
    this->blue = color&0xff;
    this->position = position;
    this->velocity = velocity;
}
*/

int Missile::CheckInWindow(void)
{
    if (position.y < 0 || position.y >= WINDOW_HEI ||
        position.x < 0 || position.x >= WINDOW_WID)
    {
        state = 0;
    }
    else
    {
        state = 1;
    }
    return state;
}

void Missile::Move(double deltaT)
{
    position += velocity * deltaT;
}

MissileType Missile::getType(){
    return type;
}

void Missile::setType(MissileType type){
    this->type = type;
}

void Missile::setColor(const Color &color){
    this->color = color;
}

void Missile::setPosition(const Vector2 &position){
    this->position = position;
}

Vector2 Missile::getPosition()
{
    return position;
}

void Missile::setVelocity(const Vector2 &velocity){
    this->velocity = velocity;
}

void Missile::setDirection(const Vector2 &direction) {
    double vel = velocity.length();
    this->direction = direction;
    this->velocity = direction * vel;
}

void Missile::setState(int state){
    this->state =  state;
}

int Missile::getState(){
    return state;
}


void Missile::Launch(Vector2 position)
{
    if(state == 0)
    {
        state = 1;
        this->position = position;
    }
}

void Missile::Move(Vector2 newPosition)
{
    position = newPosition;
}



void Bullet::Draw() {
    if (state) {
        glColor3ubv(color.arr());
        glBegin(GL_LINES);
        glVertex2d(position.x + direction.x * len ,position.y + direction.y * len);
        glVertex2d(position.x - direction.x * len ,position.y - direction.y * len);
        glEnd();
    }
}



void Cannon::Draw() {
    if (state) {
        glBegin(GL_POLYGON);

        glColor3ubv(color.arr());
        int i;
        for(i=0; i<64; i++)
        {
            double angle=(double)i*PI/32.0;
            double x=position.x+cos(angle)*radius;
            double y=position.y+sin(angle)*radius;
            glVertex2d(x,y);
        }

        glEnd();
    }
}


void Laser::setPlane(Plane *plane) {
    this->plane = plane;
}


void Laser::Draw() {
    if (state) {
        //TODO: set up shade model in game engine when possible
        glShadeModel(GL_SMOOTH);

        double dirX = direction.x * WINDOW_DIAG;
        double dirY = direction.y * WINDOW_DIAG;
        double widX = direction.y * width;
        double widY = -direction.x * width;

        glBegin(GL_QUADS);

        glColor3ubv(color.arr());
        glVertex2d(position.x + widX, position.y + widY);
        glVertex2d(position.x + widX + dirX, position.y + widY + dirY);

        glColor3ubv(gWhite.arr());
        glVertex2d(position.x + dirX, position.y + dirY);
        glVertex2d(position.x, position.y);


        glVertex2d(position.x + dirX, position.y + dirY);
        glVertex2d(position.x, position.y);

        glColor3ubv(color.arr());
        glVertex2d(position.x - widX, position.y - widY);
        glVertex2d(position.x - widX + dirX, position.y - widY + dirY);


        glEnd();
        
        /* Electricity effect */
        glColor3ub(250, 250, 255);
        glBegin(GL_LINE_STRIP);
        
        double randX = 0.0, randY = 0.0;
        Vector2 temp(position);
        Vector2 point;
        Vector2 perpendicular(direction.y, -direction.x);
        /* generate electricity until outside screen */
        while (temp.x > 0 && temp.x < WINDOW_WID 
            && temp.y > 0 && temp.y < WINDOW_HEI) {
            randX = rand()%(2*(int)width+8) - width - 4;
            randY = rand() % 80;
            
            point = temp + perpendicular * randX; 
            glVertex2d(point.x, point.y);
            temp += direction * randY;
            
            /* random break */
            if (rand()%5 == 0) {
                glEnd();
                glBegin(GL_LINE_STRIP);
            }
        }

        glEnd();
    }
}


int Laser::CheckInWindow() {
    if(countDown <= 0)
        state = 0;
    else 
        state = 1;
    return state;
}

void Laser::Move(double deltaT) {
    /* generally should not happen */
    if (!plane) return;
    
    direction = plane->direction;
    position = plane->getMissilePos();
}

Missile *CopyMissile(Missile *missile) {
    if (missile == NULL) return NULL;
    
    Missile *newMissile;
    switch (missile->getType()) {
        case BULLET:
            newMissile = new Bullet(*(Bullet *)missile);
            break;
            
        case CANNON:
            newMissile = new Cannon(*(Cannon *)missile);
            break;
            
        case LASER:
            newMissile = new Laser(*(Laser *)missile);
            break;
    }
    assert(newMissile);
    return newMissile;
}


#ifdef MISSILE_DEBUG
int main(void)
{
    Vector2 bulletV(0, -10);
    Vector2 cannonV(0, -20);
    Vector2 laserV(0, -1);
    Vector2 bulletPosition(300, 400);
    Vector2 laserPosition(100, 400);
    Vector2 cannonPosition(200, 400);
    Vector2 velocity(1, 1);
    MissileList missiles;
    Missile *missile;
    MissileNode *laser = NULL;
    int laserReload = 0;
    int cannonReload = 0;
    int bulletReload = 0;
    int firing = 0;

    FsOpenWindow(0,0,600, 800,1);
    glClearColor(0, 0, 0, 1);

    bool running = true;

    while(running)
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        FsPollDevice();

        int key=FsInkey();
        int lb, mb, rb, mx, my;
        int mouse = FsGetMouseEvent(lb, mb, rb, mx, my);

        if(FSKEY_ESC==key)
        {
            running=false;
        }
        if(FsGetKeyState(FSKEY_UP))
        {
            bulletV.rotate(0.05);
            cannonV.rotate(0.05);
            laserV.rotate(0.05);
        }
        if(FsGetKeyState(FSKEY_DOWN))
        {
            bulletV.rotate(-0.05);
            cannonV.rotate(-0.05);
            laserV.rotate(-0.05);
        }
        
        if(FSMOUSEEVENT_LBUTTONDOWN == mouse) {
            firing = 1;
        } else if (FSMOUSEEVENT_LBUTTONUP == mouse) {
            firing = 0;
        }
            
        if (firing == 1) {
            if (laserReload <= 0) {
                laserReload = 100;
                missile = new Laser(gBlue, 100, laserPosition, laserV, 1);
                laser = missiles.InsertFront(missile);
            }
            if (bulletReload <= 0) {
                bulletReload = 100;
                missile = new Bullet(gRed, 100, bulletPosition, bulletV, 1);
                missiles.InsertFront(missile);
            }
            if (cannonReload <= 0) {
                cannonReload = 100;
                missile = new Cannon(gGreen, 100, cannonPosition, cannonV, 1);
                missiles.InsertFront(missile);
            }
        }
        else if (firing == 0 && laser) {
            missiles.Delete(laser);
            laser = NULL;
            laserReload = 0;
        }

        MissileNode *node;
        node = missiles.getFront();
        while(node) {
            node->dat->Move(1.0);
            node->dat->Draw();
            if (!node->dat->CheckInWindow()) {
                node = missiles.Delete(node);
            } else {
                node = node->next;
            }
        }
        
        if (bulletReload > 0) bulletReload -= 10;
        if (cannonReload > 0) cannonReload -= 10;

/*
if(key == FSKEY_UP) printf("UP!!!!\n");
if(key == FSKEY_SPACE) printf("SPACE!!!\n");
if (FsCheckKeyHeldDown()) printf("KEY DOWN!!!\n");
*/
        FsSwapBuffers();


        FsSleep(25);
    }

    return 0;
}
#endif
