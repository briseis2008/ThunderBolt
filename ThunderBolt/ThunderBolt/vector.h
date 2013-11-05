#ifndef ThunderBolt_Vector_h
#define ThunderBolt_Vector_h

#include <math.h>

class Vector {
public:
    double x;
    double y;
    
public:
    Vector(): x(0.0), y(0.0) { }
    Vector(double x, double y) {this->x = x; this->y = y;}
    
    const Vector operator+ (const Vector &rhs) const;
    const Vector operator- (const Vector &rhs) const;
    const Vector operator* (const double rhs) const;
    const Vector operator/ (const double rhs) const;
    
    inline void set(double x, double y) {this->x = x; this->y = y;}
    inline void negate() {x = -x; y = -y;}
    
    inline double dot(const Vector &v2) const {
        return x * v2.x + y * v2.y;
    }
    inline double length() const {
        return sqrt(x * x + y * y);
    }
    inline double distance(const Vector &v2) const {
        return sqrt(pow((this->x - v2.x), 2) + pow((this->y - v2.y), 2));
    }
};

#endif
