#include "texture_loader.h"
#include <assert.h>
#include "debug.h"


TextureLoader::TextureLoader() {
    this->texture = 0;
}

TextureLoader::~TextureLoader() {
    if (texture) {
        glDeleteTextures(1, &texture);
    }
}

/* load the png into memory, return 0 if succeeded, 1 otherwise */
int TextureLoader::LoadPng(const char *fileName) {
    assert(fileName);
    
    if (YSOK != Decode(fileName)) {
        printf("Failed to load PNG file: \"%s\"\n", fileName);
        return 1;
    }
    Flip();
    printf("Load PNG %s succesfully!\twid [%d] hei [%d]\n", fileName, wid, hei);
    return 0;
}

/* Bind current texture */
void TextureLoader::Bind() {
    glBindTexture(GL_TEXTURE_2D, texture);
}

/* load a PNG file as texture. Return 0 if succedded, 1 otherwise */
int TextureLoader::LoadTexture(const char *fileName) {
    if (LoadPng(fileName)) return 1;
    
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wid, hei, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, rgba);
    glDisable(GL_TEXTURE_2D);

    return 0;
}


/* render the texture in full size centered at Vector2 &position. */
void TextureLoader::Draw(const Vector2 &position) {
    /* bind texture first */
    glEnable(GL_TEXTURE_2D);
    Bind();
    double halfW = wid / 2.0;
    double halfH = hei / 2.0;
    glBegin(GL_QUADS);
    /* lower left */
    glTexCoord2f(0.0, 0.0);
    glVertex2d(position.x - halfW, position.y + halfH);
    /* upper left */
    glTexCoord2f(0.0, 1.0);
    glVertex2d(position.x - halfW, position.y - halfH);
    /* upper right */
    glTexCoord2f(1.0, 1.0);
    glVertex2d(position.x + halfW, position.y - halfH);
    /* lower right */
    glTexCoord2f(1.0, 0.0);
    glVertex2d(position.x + halfW, position.y + halfH);
    
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

/* render the texture in full size centered at position, with direction
   direction has to be normalized. */
void TextureLoader::Draw(const Vector2 &position, const Vector2 &direction) {
    Draw(position, direction, wid, hei);
}

/* render the texture in specified size, centered at position, with direction
   direction has to be normalized */
void TextureLoader::Draw(const Vector2 &position, const Vector2 &direction, 
                         int width, int height) {
    /* bind texture first */
    glEnable(GL_TEXTURE_2D);
    Bind();
    double halfW = width / 2.0;
    double halfH = height / 2.0;
    glBegin(GL_QUADS);
    /* lower left */
    glTexCoord2f(0.0, 0.0);
    glVertex2d(position.x - direction.x * halfH + direction.y * halfW, 
               position.y - direction.y * halfH - direction.x * halfW);
    /* upper left */
    glTexCoord2f(0.0, 1.0);
    glVertex2d(position.x + direction.x * halfH + direction.y * halfW, 
               position.y + direction.y * halfH - direction.x * halfW);
    /* upper right */
    glTexCoord2f(1.0, 1.0);
    glVertex2d(position.x + direction.x * halfH - direction.y * halfW, 
               position.y + direction.y * halfH + direction.x * halfW);
    /* lower right */
    glTexCoord2f(1.0, 0.0);
    glVertex2d(position.x - direction.x * halfH - direction.y * halfW, 
               position.y - direction.y * halfH + direction.x * halfW);
    
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

/* Wrapper of glewInit(). Needed for texture rendering.
 * Solved cross-platform issue on Mac OS.
 */
void textureInit() {
    /* As mentioned in header file, glewInit() is not needed on Mac OS!
     * Hello Steve!! */
    #if defined(__APPLE__)
        FsChangeToProgramDir();
    #else
        glewExperimental=TRUE;
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            printf("Error: %s\n", glewGetErrorString(err));
            exit(1);
        }
    #endif    
}




#ifdef TEXTURE_LOADER_DEBUG
int main() {
    FsOpenWindow(0,0,600, 800,1);
    glClearColor(0, 0, 0, 1);    
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    textureInit();

    TextureLoader loader;
    loader.LoadTexture("pics/explosion.png");
    
    int state = 0;
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
        if (FSMOUSEEVENT_LBUTTONDOWN == mouse) {
            state++;
            state = state % 4;
        }
        glColor4f(1,1 , 1,0.5);
        
        switch (state) {
            case 0:
                loader.Draw(Vector2(300,400));
                break;
            case 1:
                loader.Draw(Vector2(300,400), Vector2(0.7071, 0.7071));
                break;
            case 2:
                loader.Draw(Vector2(300,400), Vector2(0.7071, 0.7071), 512, 512);
                break;
            case 3:
                loader.Draw(Vector2(300,400), Vector2(1, 0), 512, 512);
                break;
            
        }
        
        
/*        glRasterPos2d(300, 700);
        glDrawPixels(256, 256,GL_RGBA,GL_UNSIGNED_BYTE,loader.rgba);
*/        
        FsSwapBuffers();
        FsSleep(100);
        
    }   
    return 0;
}
#endif
