#pragma once

#include <fstream>
#include <cmath>

#include "Utility_functions.h"

class Vec {
	public:
		Vec() : e{0, 0, 0} {}
		Vec(float e0, float e1, float e2) : e{e0, e1, e2} {}

		float getX() const { return e[0]; }
		float getY() const { return e[1]; }
		float getZ() const { return e[2]; }

		/* Utility functions to set random Vec */

		inline static Vec random() {

			return Vec(random_float(), random_float(), random_float());
		}

		inline static Vec random(float min, float max) {

			return Vec(random_float(min, max), random_float(min, max), random_float(min, max));
		}

		bool near_zero() const {
			// Return true if the vector is close to zero in all dimensions.
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

/* Utility function to set random point in sphere */

Vec random_in_unit_sphere() {
	
	for(;;) {
		Vec point = Vec::random(-1, 1);
		
		if (point.length_squared() >= 1) { continue; }
		
		return point;
	}
}

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
