#include "color.h"

/* default to non-transparent black */
Color::Color() {
    array[0] = 0;
    array[1] = 0;
    array[2] = 0;
    array[3] = 255;
}


Color::Color(unsigned char r, unsigned char g, unsigned char b) {
    array[0] = r;
    array[1] = g;
    array[2] = b;
    array[3] = 255;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    array[0] = r;
    array[1] = g;
    array[2] = b;
    array[3] = a;
}

void Color::set(unsigned char r, unsigned char g, unsigned char b) {
    array[0] = r;
    array[1] = g;
    array[2] = b;
}

void Color::set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    array[0] = r;
    array[1] = g;
    array[2] = b;
    array[3] = a;
}

unsigned char Color::r() {
    return array[0];
}

unsigned char Color::g() {
    return array[1];
}

unsigned char Color::b() {
    return array[2];
}

unsigned char Color::a() {
    return array[3];
}

const unsigned char* Color::arr() const {
    return array;
}


/* Global defined colors */
const Color gBlack(0, 0, 0);
const Color gWhite(255, 255, 255);
const Color gRed(255, 0, 0);
const Color gGreen(0, 255, 0);
const Color gBlue(0, 0, 255);
const Color gYellow(255, 255, 0);
const Color gPurple(255, 0, 255);
