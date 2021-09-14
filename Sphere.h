#pragma once

#include "Hittable.h"
#include "Vec.h"

class Sphere : public Hittable {
	public:
		Sphere() {}
		Sphere(Point center, float radius, shared_ptr<Material> material) : center(center), radius(radius), material_ptr(material) {};

		virtual bool hit(
			const Ray &ray, float distance_min, float distance_max, hit_record &record) const override;
		
		virtual bool bounding_box(float s_time, float e_time, AABB& output_box) const override;

	private: 
		static void get_sphere_uv(const Point &point, float &u, float &v) {
			// point: a given point on the sphere of radius one, centered at the origin.
			// u: returned value [0,1] of angle around the Y axis from X=-1.
			// v: returned value [0,1] of angle from Y=-1 to Y=+1.
			//     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
			//     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
			//     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

			float theta = acos(-point.getY());
			float phi = atan2(-point.getZ(), point.getX()) + pi;

			u = phi / (2*pi);
			v = theta / pi;
		}

	public:
		Point center;                                                                                           // Sphere: center
		float radius;                                                                                           // Sphere: radius
		shared_ptr<Material> material_ptr;                                                                      // Sphere: material
};

bool Sphere::hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const {

	Vec oc = ray.get_origin() - center;                                                                         // Vec OC: camera to center
	
	float a = ray.get_direction().length_squared();                                                             // Discriminant: a: ray.dir * sqrt(length)
	float half_b = dot(oc, ray.get_direction());                                                                // Discriminant: half b:
	float c = oc.length_squared() - radius * radius;                                                            // Discriminant: c: OC.sqrt(length) - sqrt(radius)

	float discriminant = half_b * half_b - a * c;                                                               // Discriminant: with half b
	
	if (discriminant < 0) { return false; }                                                                     // Hit: fail: The sphere was not hit

	float sqrt_discriminant = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	float root = (-half_b - sqrt_discriminant) / a;                                                             // Quadratic equation: - version: first hit point
	
	if (root < distance_min || distance_max < root) {                                                           // Hit: if fist point out of range
		root = (-half_b + sqrt_discriminant) / a;                                                               // Quadratic equation: + version: second hit point

		if (root < distance_min || distance_max < root) { return false; }                                       // Hit: fail: second hit point out of range
	}

	record.distance = root;
	record.point = ray.at(record.distance);
	record.normal = (record.point - center) / radius;
	record.material_ptr = material_ptr;

	Vec outward_normal = record.normal;
	record.set_face_normal(ray, outward_normal);
	get_sphere_uv(outward_normal, record.u, record.v);

	return true;
}

bool Sphere::bounding_box(float s_time, float e_time, AABB &output_box) const {
	
	output_box = AABB(
		center - Vec(radius, radius, radius),
		center + Vec(radius, radius, radius));
	
	return true;
}
