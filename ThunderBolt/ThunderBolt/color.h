#ifndef THUNDERBOLD_COLOR_H
#define THUNDERBOLD_COLOR_H

class Color {
    unsigned char array[4];
public:
    Color();
    Color(unsigned char r, unsigned char g, unsigned char b);
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    void set(unsigned char r, unsigned char g, unsigned char b) ;
    void set(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    unsigned char r() ;
    unsigned char g() ;
    unsigned char b() ;
    unsigned char a() ;
    const unsigned char* arr() const;
};


/* global predefined colors */
extern const Color gBlack;
extern const Color gWhite;
extern const Color gRed;
extern const Color gGreen;
extern const Color gBlue;
extern const Color gYellow;
extern const Color gPurple;


#endif

