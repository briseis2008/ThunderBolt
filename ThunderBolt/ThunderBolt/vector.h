#ifndef ThunderBolt_Vector_h
#define ThunderBolt_Vector_h

#include <math.h>

class Vector2 {
public:
    double x;
    double y;
    
public:
    Vector2(): x(0.0), y(0.0) { }
    Vector2(double x, double y) {this->x = x; this->y = y;}

    Vector2& operator+= (const Vector2 &rhs);
    Vector2& operator-= (const Vector2 &rhs);
    Vector2& operator*= (const double rhs);
    Vector2& operator/= (const double rhs);
    
    const Vector2 operator+ (const Vector2 &rhs) const;
    const Vector2 operator- (const Vector2 &rhs) const;
    const Vector2 operator* (const double rhs) const;
    const Vector2 operator/ (const double rhs) const;
    
    inline void set(double x, double y) {this->x = x; this->y = y;}
    inline void negate() {x = -x; y = -y;}
    
    inline double dot(const Vector2 &v2) const {
        return x * v2.x + y * v2.y;
    }
    inline double length() const {
        return sqrt(x * x + y * y);
    }
    inline double distance(const Vector2 &v2) const {
        return sqrt(pow((this->x - v2.x), 2) + pow((this->y - v2.y), 2));
    }
    void normalize();
    void rotate(double rad);

};

extern const Vector2 gZero;

#endif
