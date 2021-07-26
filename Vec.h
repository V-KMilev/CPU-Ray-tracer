#pragma once

#include <cmath>
#include <fstream>

class Vec {
    public:
        Vec() : e{0, 0, 0} {}
        Vec(float e0, float e1, float e2) : e{e0, e1, e2} {}

        float getX() const { return e[0]; }
        float getY() const { return e[1]; }
        float getZ() const { return e[2]; }

        Vec operator -() const { return Vec(-e[0], -e[1], -e[2]); }
        
        float operator [] (int i) const { return e[i]; }
        float& operator [] (int i) { return e[i]; }

        Vec& operator += (const Vec& v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];

            return *this;
        }

        Vec& operator *= (const float number) {
            
            e[0] *= number;
            e[1] *= number;
            e[2] *= number;
            
            return *this;
        }

        Vec& operator /= (const float number) {
            return *this *= 1 / number;
        }

        float length_squared() const {
            return e[0] * e[0] +
                   e[1] * e[1] +
                   e[2] * e[2];
        }

        float length() const {
            return sqrt(length_squared());
        }

    public:
        float e[3];
};

using Point = Vec;                                                                  // 3D point
using Color = Vec;                                                                  // RGB color

// Vec Utility Functions

inline std::ostream& operator << (std::ostream &out, const Vec &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec operator + (const Vec &u, const Vec &v) {
    return Vec(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec operator - (const Vec &u, const Vec &v) {
    return Vec(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec operator * (const Vec &u, const Vec &v) {
    return Vec(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec operator * (float distance, const Vec &v) {
    return Vec(distance * v.e[0], distance * v.e[1], distance * v.e[2]);
}

inline Vec operator * (const Vec &v, float distance) {
    return v * distance;
}

inline Vec operator / (Vec v, float distance) {
    return (1 / distance) * v;
}

inline float dot(const Vec &u, const Vec &v) {
    return u.e[0] * v.e[0] + 
           u.e[1] * v.e[1] +
           u.e[2] * v.e[2];
}

inline Vec cross(const Vec &u, const Vec &v) {
    return Vec
    (
        u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]
    );
}

inline Vec unit_vector(Vec v) {
    return v / v.length();
}

// Write the translated [0,255] value of each color component.
void write_color(std::ofstream& out, Color pixel_color) {
    
    out << static_cast<int>(255.999 * pixel_color.getX())   << ' '
        << static_cast<int>(255.999 * pixel_color.getY())   << ' '
        << static_cast<int>(255.999 * pixel_color.getZ())   << '\n';
}
