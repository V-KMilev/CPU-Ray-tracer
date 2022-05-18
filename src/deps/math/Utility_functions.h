#pragma once

#include <cstdlib>
#include <random>
#include <memory>
#include <limits>
#include <cmath>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

/* Constants: */
const double infinity = std::numeric_limits<float>::infinity();
const double pi = 3.1415926535897932385;
const float epsilon = 1e-3f;

/*
 * Function - degrees_to_radians
 *
 * Parameters:
 * [p] float degrees
 * 
 * Return type:
 * float
 * 
 * Use:
 * By calling degrees_to_radians you get 
 * in return your degrees as radians.
 */

inline float degrees_to_radians(float degrees) {
	return degrees * pi / 180.0;
}

/*
 * Function - random_float
 *
 * Parameters:
 * none
 * 
 * Return type:
 * float
 * 
 * Use:
 * By calling random_float you get 
 * in return a random float in [0,1).
 */

inline float random_float() {
	return rand() / (RAND_MAX + 1.0);
}

/*
 * Function - random_float
 *
 * Parameters:
 * none
 * 
 * Return type:
 * float
 * 
 * Use:
 * By calling random_float you get 
 * in return a random float in [min,max).
 */

inline float random_float(float min, float max) {
	return min + (max - min) * random_float();
}

/*
 * Function - random_int
 *
 * Parameters:
 * [p] int min,
 * [p] int max
 * 
 * Return type:
 * int
 * 
 * Use:
 * By calling random_int you get 
 * in return a random integer in [min,max].
 */

inline int random_int(int min, int max) {
	return static_cast<int>(random_float(min, max+1));
}

/*
 * Function - clamp
 *
 * Parameters:
 * [p] float x,
 * [p] float min,
 * [p] double max
 * 
 * Return type:
 * float
 * 
 * Use:
 * By calling clamp you get 
 * in return x clamped in the range [min,max].
 */

inline float clamp(float x, float min, double max) {
	if(x < min) { return min; }
	if(x > max) { return max; }
	return x;
}
