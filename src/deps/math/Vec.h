#pragma once

#include <fstream>
#include <cmath>

#include "Utility_functions.h"

class Vec {
	public:
		Vec() : e{0, 0, 0} {}
		Vec(float e0, float e1, float e2) : e{e0, e1, e2} {}

		/*
		 * Functions - getX, getY, getZ
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * float
		 * 
		 * Use:
		 * By calling this funtion you get 
		 * in return dimension value.
		 */

		float getX() const { return e[0]; }
		float getY() const { return e[1]; }
		float getZ() const { return e[2]; }

		/*
		 * Function - random
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * Vec
		 * 
		 * Use:
		 * By calling random you get 
		 * in return Vec with random 
		 * dimensions (X,Y,Z) values.
		 */

		inline static Vec random() {
			return Vec(random_float(), random_float(), random_float());
		}

		/*
		 * Function - random
		 *
		 * Parameters:
		 * [p] float min,
		 * [p] float max
		 * 
		 * Return type:
		 * Vec
		 * 
		 * Use:
		 * By calling random you get 
		 * in return Vec with random 
		 * coordinates (X,Y,Z) values 
		 * in range [min,max].
		 */

		inline static Vec random(float min, float max) {
			return Vec(random_float(min, max), random_float(min, max), random_float(min, max));
		}

		/*
		 * Function - near_zero
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * bool
		 * 
		 * Use:
		 * By calling near_zero you get 
		 * in return 1 (true) if the Vec
		 * is close to zero in all dimensions.
		 */

		bool near_zero() const {
			const float close_to_zero = 1e-8;
			return (fabs(e[0]) < close_to_zero) && (fabs(e[1]) < close_to_zero) && (fabs(e[2]) < close_to_zero);
		}

		Vec operator -() const { return Vec(-e[0], -e[1], -e[2]); }

		float operator [] (int i) const { return e[i]; }
		float& operator [] (int i) { return e[i]; }

		Vec& operator += (const Vec &v) {
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

		/*
		 * Function - length_squared
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * float
		 * 
		 * Use:
		 * By calling length_squared you get 
		 * in return the sum of the squres of 
		 * all dimensions of the Vec.
		 */

		float length_squared() const {
			return e[0] * e[0] +
				   e[1] * e[1] +
				   e[2] * e[2];
		}

		/*
		 * Function - length
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * float
		 * 
		 * Use:
		 * By calling length you get 
		 * in return the squared sum of 
		 * the squres of all dimensions of the Vec.
		 */

		float length() const {
			return sqrt(length_squared());
		}

	public:
		float e[3];
};

using Point = Vec;    // 3D point

/* Vec Utility Functions: */

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
	return distance * v;
}

inline Vec operator / (Vec v, float distance) {
	return (1 / distance) * v;
}

/*
 * Function - dot
 *
 * Parameters:
 * [p] const Vec &u, 
 * [p] const Vec &v
 * 
 * Return type:
 * float
 * 
 * Use:
 * By calling dot you get 
 * in return the dot prduct of v and u. 
 * https://www.youtube.com/watch?v=h0NJK4mEIJU.
 */

inline float dot(const Vec &u, const Vec &v) {
	return u.e[0] * v.e[0] +
		   u.e[1] * v.e[1] +
		   u.e[2] * v.e[2];
}

/*
 * Function - cross
 *
 * Parameters:
 * [p] const Vec &u, 
 * [p] const Vec &v
 * 
 * Return type:
 * Vec
 * 
 * Use:
 * By calling cross you get 
 * in return the cross prduct of v and u. 
 * https://www.youtube.com/watch?v=h0NJK4mEIJU.
 */

inline Vec cross(const Vec &u, const Vec &v) {
	return Vec
	(
		u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]
	);
}

/*
 * Function - unit_vector
 *
 * Parameters:
 * [p] Vec v
 * 
 * Return type:
 * Vec
 * 
 * Use:
 * By calling unit_vector you get 
 * in return normed version of v. 
 * Normed Vec - magnitude of 1.
 */

inline Vec unit_vector(Vec v) {
	return v / v.length();
}

/*
 * Function - random_in_unit_sphere
 *
 * Parameters:
 * none
 * 
 * Return type:
 * Point
 * 
 * Use:
 * By calling random_in_unit_sphere you get 
 * in return random Point in sphere range 
 * from center Point.
 */

Point random_in_unit_sphere() {

	for(;;) {
		Vec point = Vec::random(-1, 1);

		if (point.length_squared() >= 1) { continue; }

		return point;
	}
}

/*
 * Function - random_unit_vector
 *
 * Parameters:
 * none
 * 
 * Return type:
 * Vec
 * 
 * Use:
 * By calling random_unit_vector you get 
 * in return normed version 
 * of random_in_unit_sphere Vec.
 */

Vec random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

/* Camera disk offset math */
Vec random_in_unit_disk() {

	for(;;) {
		Vec point = Vec(random_float(-1.0 ,1.0), random_float(-1.0 ,1.0), 0.0);

		if (point.length_squared() >= 1.0) { continue; }

		return point;
	}
}
