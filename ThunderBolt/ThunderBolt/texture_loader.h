#ifndef THUNDERBOLT_TEXTURE_LOADER_H
#define THUNDERBOLT_TEXTURE_LOADER_H

/* Annoying bug: Mac OS does not need glew.h, since OpenGL is not an extension
 *               on Mac.
 *
 * Thanks to Viktor!
 * http://stackoverflow.com/questions/11212347/glewinit-fails-on-macos
 */
#ifndef __APPLE__
    #include <GL/glew.h>
#endif

#include "vector.h"
#include <stdio.h>
#include "yspng.h"
#include "fssimplewindow.h"

class TextureLoader : public YsRawPngDecoder {
    GLuint texture;
    int LoadPng(const char *fileName);
    
public:
    TextureLoader();
    ~TextureLoader();
    
    void Bind();
    int LoadTexture(const char *fileName);
    void Draw(const Vector2 &positoin);
    void Draw(const Vector2 &position, const Vector2 &direction);
    void Draw(const Vector2 &position, const Vector2 &direction, 
              int width, int height);
};

void textureInit();

#endif
