#pragma once

#include "Vec.h"

class Ray {
	public:
		Ray() {}
		Ray(const Point &origin, const Vec &directon, float time) : origin(origin), directon(directon), time(time) {}

		/*
		 * Function - get_origin
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * Point
		 * 
		 * Use:
		 * By calling get_origin you get 
		 * in return the origin point of the Ray.
		 */

		Point get_origin() const { return origin; }

		/*
		 * Function - get_direction
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * Vec
		 * 
		 * Use:
		 * By calling get_direction you get 
		 * in return the direction the Ray 
		 * is heading.
		 */

		Vec get_direction() const { return directon; }

		/*
		 * Function - get_time
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * float
		 * 
		 * Use:
		 * By calling get_time you get 
		 * in return at what time point 
		 * we have launched the Ray.
		 */

		float get_time() const { return time; }

		/*
		 * Function - at
		 *
		 * Parameters:
		 * [p] float distance
		 * 
		 * Return type:
		 * Point
		 * 
		 * Use:
		 * By calling at you get 
		 * in return Point in the 
		 * 3D world.
		 */

		Point at(float distance) const {

			return origin + distance * directon;
		}

	public:
		Point origin;    // Ray: original start point
		Vec directon;    // Ray: directon

		float time;      // Ray: time
};
