#pragma once

#include "Utility_functions.h"

#include "Hittable_list.h"
#include "Hittable.h"

class BVH_node : public Hittable {
	public:
		BVH_node();

		BVH_node(const Hittable_list &list, float s_time, double e_time)
			: BVH_node(list.objects, 0, list.objects.size(), s_time, e_time) {}

		BVH_node(const std::vector<shared_ptr<Hittable>> &src_objects,
			size_t start, size_t end, float s_time, double e_time);

		virtual bool hit(
			const Ray &ray, float distance_min, float distance_max, hit_record &record) const override;

		virtual bool bounding_box(float s_time, float e_time, AABB &output_box) const override;

	public:
		shared_ptr<Hittable> left;
		shared_ptr<Hittable> right;
		AABB box;
};

bool BVH_node::bounding_box(float s_time, float e_time, AABB &output_box) const {
	output_box = box;
	return true;
}

bool BVH_node::hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const {
	if (!box.hit(ray, distance_min, distance_max))
		return false;

	bool hit_left = left->hit(ray, distance_min, distance_max, record);
	bool hit_right = right->hit(ray, distance_min, hit_left ? record.distance : distance_max, record);

	return hit_left || hit_right;
}