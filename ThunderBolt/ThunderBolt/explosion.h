#ifndef THUNDERBOLT_EXPLOSION_H
#define THUNDERBOLT_EXPLOSION_H

#include "vector.h"
#include "linked_list.h"
#include "texture_loader.h"

enum ExplosionType {
    E_PLANE
};

/* used to mimic explosion effect, by playing back consecutive frames */
class Animation {
    /* center of explostion */
    Vector2 position;
    /* picture display size */
    double wid, hei;
    double halfW, halfH;
    
    /* keep track of current frame */
    int frame;
    /* how many frames are there in play back */
    int totalFrame;
    /* dimension of texture (totalFrame = frameNum * frameNum) */
    int frameNum;
    /* texture width of one single frame. frameWidth * frameNum = 1.0 */
    double frameWidth;
    
    /* controls playback speed, the larger the slower */
    int playBackGap;
    /* a play back time counter */
    int playBackTicker;
    
public:
    Animation(const Vector2 &position, double wid, double hei, int totalFrame,
              int playBackGap = 1);
    void Draw();
    int Evolve();
};


class PlaneExplosion : public Animation {
public:
    PlaneExplosion(const Vector2 &position)
       : Animation(position, 256, 256, 16, 2) {};
};


class ExplosionEffect {
    /* list of all explosions */
    DoublyLinkedList<Animation *> explosions;
    /* one list share the same texture */
    TextureLoader texture;
    
    ExplosionType type;
public:
    ExplosionEffect(const char *fileName, ExplosionType type);
                    
    void Explode(const Vector2 &position);
    void Play();
};

#endif
