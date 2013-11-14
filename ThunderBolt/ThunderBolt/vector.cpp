#include "vector.h"
#include <assert.h>

Vector2& Vector2::operator+= (const Vector2 &rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}

Vector2& Vector2::operator-= (const Vector2 &rhs) {
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
}

Vector2& Vector2::operator*= (const double rhs) {
    this->x *= rhs;
    this->y *= rhs;
    return *this;
}

Vector2& Vector2::operator/= (const double rhs) {
    assert(rhs != 0.0);
    
    this->x /= rhs;
    this->y /= rhs;
    return *this;
}

const Vector2 Vector2::operator+ (const Vector2 &rhs) const {
    Vector2 result = *this;
    result += rhs;
    return result;
}

const Vector2 Vector2::operator- (const Vector2 &rhs) const {
    Vector2 result = *this;
    result -= rhs;
    return result;
}

const Vector2 Vector2::operator* (const double rhs) const {
    Vector2 result = *this;
    result *= rhs;
    return result;
}
const Vector2 Vector2::operator/ (const double rhs) const {
    assert(rhs != 0.0);
    Vector2 result = *this;
    result /= rhs;
    return result;
}

void Vector2::normalize() {
    double len = length();
    if (len == 0) return;
    x = x / len;
    y = y / len;
}


void Vector2::rotate(double rad) {
    double cosTheta = cos(rad);
    double sinTheta = sin(rad);
    double newX = x * cosTheta - y * sinTheta;
    double newY = x * sinTheta + y * cosTheta;
    x = newX;
    y = newY;
}

const Vector2 gZero(0.0, 0.0);
