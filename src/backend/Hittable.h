#pragma once

#include "Ray.h"

class Material;

struct hit_record {
	float distance;                                                   // hit_record: distance
	float u;                                                          // hit_record: unwrapping u
	float v;                                                          // hit_record: uvwrapping v
	Point point;                                                      // hit_record: hit point
	Vec normal;                                                       // hit_record: parale
	
	shared_ptr<Material> material_ptr;                                // hit_record: material hitted

	bool front_face;                                                  // hit_record: parallel to the tangent

	inline void set_face_normal(const Ray &ray, const Vec &outward_normal) {

		front_face = dot(ray.get_direction(), outward_normal) < 0;    // hit_record: if is in the sphere
		normal = front_face ? outward_normal : -outward_normal;       // hit_record: sets it to go in the sphere
	}
};

class Hittable {    // Interface
	public:
		virtual bool hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const = 0;
};