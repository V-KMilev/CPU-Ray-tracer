#pragma once

#include "Hittable.h"
#include "Vec.h"

class Sphere : public Hittable {
	public:
		Sphere() {}
		Sphere(Point center, float radius, shared_ptr<Material> material) : center(center), radius(radius), material_ptr(material) {};

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
		 * If ray hits Sphere hit returns 1 (true), 
		 * else 0 (false).
		 */

		virtual bool hit(
			const Ray &ray, float distance_min, float distance_max, hit_record &record) const override;

	private:

		/*
		 * Function - get_sphere_uv
		 *
		 * Parameters:
		 * [p] const Point &point, 
		 * [p] float &u, 
		 * [p] float &v
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * point: a given point on the sphere of radius one, centered at the origin.
		 * u: returned value [0,1] of angle around the Y axis from X=-1.
		 * v: returned value [0,1] of angle from Y=-1 to Y=+1.
		 *   <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
		 *   <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
		 *   <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>
		 */

		static void get_sphere_uv(const Point &point, float &u, float &v) {

			float theta = acos(-point.getY());
			float phi = atan2(-point.getZ(), point.getX()) + pi;

			u = phi / (2 * pi);
			v = theta / pi;
		}

	public:
		Point center;                         // Sphere: center
		float radius;                         // Sphere: radius
		shared_ptr<Material> material_ptr;    // Sphere: material
};

bool Sphere::hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const {
	// quadratic equation: simplified
	// normal quadratic equation in case:
	// -(2 * (oc * ray.dir)) +- root(sqrt(os * ray.dir) - 4 * (ray.dir * sqrt(length)) * (oc.sqrt(length) - sqrt(radius))) / 2 * (ray.dir * sqrt(length))
	// represented: -b +- root(sqrt(b) - 4 * a * c) / 2 * a
	//
	// simplified quadratic equation in case: divide all by 2
	// -(oc * ray.dir) +- root(sqrt(os * ray.dir) - (ray.dir * sqrt(length)) * (oc.sqrt(length) - sqrt(radius))) / (ray.dir * sqrt(length))
	// represented: -h +- root(sqrt(h) - a * c) / a where h = b / 2

	Vec oc = ray.get_origin() - center;                                      // Vec OC: camera to center

	float a = ray.get_direction().length_squared();                          // Discriminant: a: ray.dir * sqrt(length)
	float b = dot(oc, ray.get_direction());                                  // Discriminant: b: oc * ray.dir
	float c = oc.length_squared() - radius * radius;                         // Discriminant: c: oc.sqrt(length) - sqrt(radius)

	float discriminant = b * b - a * c;                                      // Discriminant

	if (discriminant < 0) { return false; }                                  // Hit: fail: The sphere was not hit

	float sqrt_discriminant = sqrt(discriminant);

	/* Find the nearest root that lies in the acceptable range. */
	float root = (-b - sqrt_discriminant) / a;                               // Quadratic equation: - version: first hit point

	if (root < distance_min || distance_max < root) {                        // Hit: if fist point out of range
		root = (-b + sqrt_discriminant) / a;                                 // Quadratic equation: + version: second hit point

		if (root < distance_min || distance_max < root) { return false; }    // Hit: fail: second hit point out of range
	}

	record.distance = root;                               // Set -> override of the distance
	record.material_ptr = material_ptr;                   // Set -> override of the material
	record.point = ray.at(record.distance);               // Set -> override of the point
	record.normal = (record.point - center) / radius;     // Set -> override of the normal

	Vec outward_normal = record.normal;                   // Set -> override of the OWN
	record.set_face_normal(ray, outward_normal);          // Set -> override of the SFN
	get_sphere_uv(outward_normal, record.u, record.v);    // Set -> override of the UV

	return true;
}
