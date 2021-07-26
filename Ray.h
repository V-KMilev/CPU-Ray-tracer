#pragma once

#include "Vec.h"

class Ray {
    public:
        Ray() {}
        Ray(const Point& origin, const Vec& directon) : origin(origin), directon(directon) {}

        Point getOrigin() const { return origin; }
        Vec getDirection() const { return directon; }

        Point at(float distance) const {
            return origin + distance * directon;
        }

    public:
        Point origin;       // original start ponint
        Vec directon;       // directon
};