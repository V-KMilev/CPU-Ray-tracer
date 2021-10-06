#pragma once

#include "Utility_functions.h"

#include "Hittable.h"

class xy_rect : public Hittable {
	public:
		xy_rect() {}

		xy_rect(float x_start, float x_end, float y_start, float y_end, float z, shared_ptr<Material> material)
			: x_start(x_start), x_end(x_end), y_start(y_start), y_end(y_end), z(z), material(material) {};

		virtual bool hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const override;

	public:
		shared_ptr<Material> material;
		float x_start, x_end, y_start, y_end, z;
};

bool xy_rect::hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const {
	
	float distance = (z - ray.get_origin().getZ()) / ray.get_direction().getZ();

	if (distance < distance_min || distance > distance_max) {
		return false;
	}

	float x = ray.get_origin().getX() + distance * ray.get_direction().getX();
	float y = ray.get_origin().getY() + distance * ray.get_direction().getY();
	
	if (x < x_start || x > x_end || y < y_start || y > y_end) {
		return false;
	}

	record.u = (x - x_start) / (x_end - x_start);
	record.v = (y - y_start) / (y_end - y_start);

	Vec outward_normal = Vec(0, 0, 1);

	record.set_face_normal(ray, outward_normal);
	record.material_ptr = material;
	record.point = ray.at(distance);
	record.distance = distance;

	return true;
}
	