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

class xz_rect : public Hittable {
	public:
		xz_rect() {}

		xz_rect(float x_start, float x_end, float z_start, float z_end, float y, shared_ptr<Material> material)
			: x_start(x_start), x_end(x_end), z_start(z_start), z_end(z_end), y(y), material(material) {};

		virtual bool hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const override;

	public:
		shared_ptr<Material> material;
		float x_start, x_end, z_start, z_end, y;
};

class yz_rect : public Hittable {
	public:
		yz_rect() {}

		yz_rect(float y_start, float y_end, float z_start, float z_end, float x, shared_ptr<Material> material)
			: y_start(y_start), y_end(y_end), z_start(z_start), z_end(z_end), x(x), material(material) {};


		virtual bool hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const override;

	public:
		shared_ptr<Material> material;
		float y_start, y_end, z_start, z_end, x;
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

	Vec outward_normal = Vec(0, 0, 1);    // Ray: Z

	record.set_face_normal(ray, outward_normal);
	record.material_ptr = material;
	record.point = ray.at(distance);
	record.distance = distance;

	return true;
}

bool xz_rect::hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const {
	
	float distance = (y - ray.get_origin().getY()) / ray.get_direction().getY();

	if (distance < distance_min || distance > distance_max) {
		return false;
	}

	float x = ray.get_origin().getX() + distance * ray.get_direction().getX();
	float z = ray.get_origin().getZ() + distance * ray.get_direction().getZ();
	
	if (x < x_start || x > x_end || z < z_start || z > z_end) {
		return false;
	}

	record.u = (x - x_start) / (x_end - x_start);
	record.v = (z - z_start) / (z_end - z_start);

	Vec outward_normal = Vec(0, 1, 0);    // Ray: Y

	record.set_face_normal(ray, outward_normal);
	record.material_ptr = material;
	record.point = ray.at(distance);
	record.distance = distance;

	return true;
}

bool yz_rect::hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const {

	float distance = (x - ray.get_origin().getX()) / ray.get_direction().getX();

	if (distance < distance_min || distance > distance_max) {
		return false;
	}

	float y = ray.get_origin().getY() + distance * ray.get_direction().getY();
	float z = ray.get_origin().getZ() + distance * ray.get_direction().getZ();
	
	if (y < y_start || y > y_end || z < z_start || z > z_end) {
		return false;
	}

	record.u = (y - y_start) / (y_end - y_start);
	record.v = (z - z_start) / (z_end - z_start);

	Vec outward_normal = Vec(1, 0, 0);    // Ray: X

	record.set_face_normal(ray, outward_normal);
	record.material_ptr = material;
	record.point = ray.at(distance);
	record.distance = distance;

	return true;
}