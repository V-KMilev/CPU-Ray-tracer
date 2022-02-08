#pragma once

#include "Vec.h"

class Ray {
	public:
		Ray() {}
		Ray(const Point &origin, const Vec &directon, float time) : origin(origin), directon(directon), time(time) {}

		Point get_origin() const { return origin; }
		Vec get_direction() const { return directon; }
		float get_time() const { return time; }

		Point at(float distance) const {

			return origin + distance * directon;
		}

	public:
		Point origin;    // Ray: original start point
		Vec directon;    // Ray: directon

		float time;      // Ray: time
};
