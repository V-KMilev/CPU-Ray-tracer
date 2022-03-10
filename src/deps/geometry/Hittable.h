#pragma once

#include "Ray.h"

/* Forward declaration */
class Material;

struct hit_record {
	float distance;                                                   // hit_record: distance
	float u;                                                          // hit_record: uvwrapping u
	float v;                                                          // hit_record: uvwrapping v
	Point point;                                                      // hit_record: hit point
	Vec normal;                                                       // hit_record: parale

	shared_ptr<Material> material_ptr;                                // hit_record: material hitted

	bool front_face;                                                  // hit_record: parallel to the tangent

	/*
	 * Function - set_face_normal
	 *
	 * Parameters:
	 * [p] const Ray &ray, 
	 * [p] const Vec &outward_normal
	 * 
	 * Return type:
	 * void
	 * 
	 * Use:
	 * If the normal face is set wronge, 
	 * it will correct it.
	 */

	inline void set_face_normal(const Ray &ray, const Vec &outward_normal) {

		front_face = dot(ray.get_direction(), outward_normal) < 0;    // hit_record: if is in the object
		normal = front_face ? outward_normal : -outward_normal;       // hit_record: sets it to go in the object
	}
};

class Hittable {    // Interface
	public:

		/*
		 * Function - hit
		 *
		 * Parameters:
		 * [p] const Ray &ray,
		 * [p] float distance_min,
		 * [p] float distance_max,
		 * [p] hit_record &record
		 * 
		 * Return type:
		 * bool
		 * 
		 * Use:
		 * Base funtion (virtual). 
		 * Hit_record update. 
		 * If ray hits object hit returns 1 (true), 
		 * else 0 (false).
		 */

		virtual bool hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const = 0;

		public:
			const char* object_name;
};
