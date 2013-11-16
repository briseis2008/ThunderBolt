#ifndef THUNDERBOLT_TEXTURE_LOADER_H
#define THUNDERBOLT_TEXTURE_LOADER_H

#include <stdio.h>
#include "yspng.h"
#include "fssimplewindow.h"

class TextureLoader : public YsRawPngDecoder {
    GLuint texture;
    
public:
    int LoadPng(const char *fileName);
};

#endif
