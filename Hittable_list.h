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

		void clear() { objects.clear(); }
		void add(shared_ptr<Hittable> object) { objects.push_back(object); }

		virtual bool hit(
			const Ray &ray, float distance_min, float distance_max, hit_record &record) const override;

		virtual bool bounding_box(float s_time, float e_time, AABB &output_box) const override;

	public:
		std::vector<shared_ptr<Hittable>> objects;                                                                  // vector of hitted objects
};

bool Hittable_list::hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const {
	
	hit_record temp_record;                                                                                         // temporarily hit_record
	bool hit_anything = false;                                                                                      // if anything is hit
	float closest_so_far = distance_max;                                                                            // closest hit

	for (const shared_ptr<Hittable> &object : objects) {
		
		if (object -> hit(ray, distance_min, closest_so_far, temp_record)) {
			
			hit_anything = true;
			closest_so_far = temp_record.distance;
			record = temp_record;
		}
	}

	return hit_anything;
}

bool Hittable_list::bounding_box(float s_time, float e_time, AABB &output_box) const {
	if (objects.empty()) return false;

	AABB temp_box;
	bool first_box = true;

	for (const shared_ptr<Hittable> &object : objects) {
		
		if (!object->bounding_box(s_time, e_time, temp_box)) { return false; }
		
		output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
		first_box = false;
	}

	return true;
}