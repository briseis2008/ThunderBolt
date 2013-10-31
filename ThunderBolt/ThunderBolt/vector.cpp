#include "vector.h"

const Vector Vector::operator+ (const Vector &rhs) const {
    return Vector(x + rhs.x, y + rhs.y);
}

const Vector Vector::operator- (const Vector &rhs) const {
    return Vector(x - rhs.x, y - rhs.y);
}

const Vector Vector::operator* (const double rhs) const {
    return Vector(x * rhs, y * rhs);
}
const Vector Vector::operator/ (const double rhs) const {
    if (rhs == 0) return *this;
    return Vector(x / rhs, y / rhs);
}

Vector& Vector::operator= (const Vector &rhs) {
    if (&rhs != this)
    {
        this->x = rhs.x;
        this->y = rhs.y;
    }
    return *this;
}