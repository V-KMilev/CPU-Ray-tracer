#pragma once

#include <memory>
#include <vector>

#include "Hittable.h"

using std::shared_ptr;
using std::make_shared;

class Hittable_list : public Hittable {
	public:
		Hittable_list() {}
		Hittable_list(shared_ptr<Hittable> object) { add(object); }

		/*
		 * Function - clear
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling clear, you clear the vecotr 
		 * full of Hittable objects.
		 */

		void clear() { objects.clear(); }

		/*
		 * Function - add
		 *
		 * Parameters:
		 * [p] shared_ptr<Hittable> object
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling add, you add (push_back) new 
		 * object into the vector.
		 */

		void add(shared_ptr<Hittable> object) { objects.push_back(object); }

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
		 * Hit is Hittable based funtion. 
		 * Hit_record update. 
		 * If ray hits object hit returns 1 (true), 
		 * else 0 (false).
		 */

		virtual bool hit(
			const Ray &ray, float distance_min, float distance_max, hit_record &record) const override;

	public:
		std::vector<shared_ptr<Hittable>> objects;    // vector of hitted objects
};

bool Hittable_list::hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const {

	hit_record temp_record;                 // temporarily hit_record
	bool hit_anything = false;              // hit flag
	float closest_so_far = distance_max;    // closest hit

	for (const shared_ptr<Hittable> &object : objects) {

		if (object->hit(ray, distance_min, closest_so_far, temp_record)) {

			hit_anything = true;
			closest_so_far = temp_record.distance;
			record = temp_record;
		}
	}

	return hit_anything;
}
