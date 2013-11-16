#include "texture_loader.h"
#include <assert.h>

int TextureLoader::LoadPng(const char *fileName) {
    assert(fileName);
    
    if (YSOK != Decode(fileName)) {
        printf("Failed to load PNG file: \"%s\"\n", fileName);
        return 0;
    }
    Flip();
    return 1;
}


#ifdef TEXTURE_LOADER_DEBUG
int main() {
    TextureLoader loader;
    loader.LoadPng("explosion.png");
    return 0;
}
#endif
