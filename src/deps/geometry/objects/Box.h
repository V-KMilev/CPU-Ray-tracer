#pragma once

#include "Hittable.h"
#include "hittable_list.h"
#include "AArect.h"

class Box : public Hittable {
	public:
		Box(const Point &start, const Point &end, shared_ptr<Material> material) : start(start), end(end), material_ptr(material) {

			object_name = "Box";
			id = Object_ID::t_box;

			sides.add(make_shared<xy_rect>(start.getX(), end.getX(), start.getY(), end.getY(), end.getZ(), material_ptr));
			sides.add(make_shared<xy_rect>(start.getX(), end.getX(), start.getY(), end.getY(), start.getZ(), material_ptr));

			sides.add(make_shared<xz_rect>(start.getX(), end.getX(), start.getZ(), end.getZ(), end.getY(), material_ptr));
			sides.add(make_shared<xz_rect>(start.getX(), end.getX(), start.getZ(), end.getZ(), start.getY(), material_ptr));

			sides.add(make_shared<yz_rect>(start.getY(), end.getY(), start.getZ(), end.getZ(), end.getX(), material_ptr));
			sides.add(make_shared<yz_rect>(start.getY(), end.getY(), start.getZ(), end.getZ(), start.getX(), material_ptr));
		}

		virtual bool hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const override {
			return sides.hit(ray, distance_min, distance_max, record);
		}

		void update() {
			sides.clear();

			sides.add(make_shared<xy_rect>(start.getX(), end.getX(), start.getY(), end.getY(), end.getZ(), material_ptr));
			sides.add(make_shared<xy_rect>(start.getX(), end.getX(), start.getY(), end.getY(), start.getZ(), material_ptr));

			sides.add(make_shared<xz_rect>(start.getX(), end.getX(), start.getZ(), end.getZ(), end.getY(), material_ptr));
			sides.add(make_shared<xz_rect>(start.getX(), end.getX(), start.getZ(), end.getZ(), start.getY(), material_ptr));

			sides.add(make_shared<yz_rect>(start.getY(), end.getY(), start.getZ(), end.getZ(), end.getX(), material_ptr));
			sides.add(make_shared<yz_rect>(start.getY(), end.getY(), start.getZ(), end.getZ(), start.getX(), material_ptr));
		}

	public:
		Point start;
		Point end;

		Hittable_list sides;

		shared_ptr<Material> material_ptr;
};
