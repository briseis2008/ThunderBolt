#include "explosion.h"
#include "debug.h"

Animation::Animation(const Vector2 &position, double wid, double hei, 
                     int totalFrame, int playBackGap){
    this->position = position;
    this->wid = wid;
    this->hei = hei;
    this->totalFrame = totalFrame;
    this->playBackGap = playBackGap;
    
    this->halfW = wid / 2.0;
    this->halfH = hei / 2.0;
    this->frame = 0;
    this->frameNum = (int)sqrt(totalFrame);
    this->frameWidth = 1.0 / frameNum;
    this->playBackTicker = 0;
}


void Animation::Draw() {
    glEnable(GL_TEXTURE_2D);
    /* validation check */
    if (frame < 0 || frame >= totalFrame) return;
    
    int x = frame % frameNum;
    /* example frames in texture:
       0  1  2  3
       4  5  6  7
       8  9  10 11
       12 13 14 15  */
    int y = (frameNum - frame / frameNum - 1);
    
    glBegin(GL_QUADS);
    /* bottom left corner */
    glTexCoord2d(x * frameWidth, y * frameWidth);
    glVertex2d(position.x - halfW, position.y + halfH);

    /* top left corner */
    glTexCoord2d(x * frameWidth, (y+1) * frameWidth);
    glVertex2d(position.x - halfW, position.y - halfH);

    /* top right corner */
    glTexCoord2d((x+1) * frameWidth, (y+1) * frameWidth);
    glVertex2d(position.x + halfW, position.y - halfH);

    /* bottom right corner */
    glTexCoord2d((x+1) * frameWidth, y * frameWidth);
    glVertex2d(position.x + halfW, position.y + halfH);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

/* let the explosion evolve. Return 1 if we get to the end; 0 otherwise */
int Animation::Evolve() {
    /* Ticker will go from 0, 1, 2,... to playBackGap */
    if (++playBackTicker >= playBackGap) playBackTicker = 0;
    
    /* only change frame when ticker is 0 */
    if (0 == playBackTicker) frame++;
    
    /* the show is end */
    if (frame >= totalFrame) return 1;
    
    return 0;
}

ExplosionEffect::ExplosionEffect(const char *fileName, ExplosionType type) {
    this->type = type;
    texture.LoadTexture(fileName);
    
}

/* Add a new explosion into the effect list */
void ExplosionEffect::Explode(const Vector2 &position) {
    Animation *newE;
    switch(type) {
        case E_PLANE:
            newE = new PlaneExplosion(position);
            break;
    }
    assert(newE);
    explosions.InsertBack(newE);
}

void ExplosionEffect::Play() {
    ListNode<Animation *> *node = explosions.getFront();
    int end = 0;
    
    texture.Bind();
    while(node) {
        node->dat->Draw();
        
        end = node->dat->Evolve();
        if (end) {
            node = explosions.Delete(node);
        } else {
            node = node->next;
        }
    }
}


#ifdef EXPLOSION_DEBUG
int main() {
    FsOpenWindow(0,0,600, 800,1);
    glClearColor(0, 0, 0, 1);    
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    textureInit();
    
    ExplosionEffect effect("pic/explosion.png", E_PLANE);
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
            break;
        }
        if (FSMOUSEEVENT_LBUTTONDOWN == mouse) {
            effect.Explode(Vector2(mx, my));
        }
        
        effect.Play();
    
        FsSwapBuffers();
        FsSleep(25);
    }
    printf("I am done\n");
    return 0;
}

#endif

