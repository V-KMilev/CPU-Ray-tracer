#pragma once

#include "Utility_functions.h"
#include "Ray.h"
#include "Vec.h"

struct hit_record;

class Material {																				// Abstract Class
	public:
		virtual bool scatter(
			const Ray &ray_in, const hit_record &record, Color &attenuation, Ray &scattered
		) const = 0;
};

class Lambertian : public Material {
	public:
		Lambertian(const Color &albedo) : albedo(albedo) {}

		virtual bool scatter(
			const Ray &ray_in, const hit_record &record, Color &attenuation, Ray &scattered) const override {
			
			Vec scatter_direction = record.normal + random_unit_vector();

			// Catch degenerate scatter direction
			if (scatter_direction.near_zero()) {
				scatter_direction = record.normal;
			}

			scattered = Ray(record.point, scatter_direction);
			attenuation = albedo;
			
			return true;
		}

	public:
		Color albedo;																			// Characterizes the reflectivity of the surface of objects

};

class Metal : public Material {
	public:
		Metal(const Color &albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1.0 ? fuzz : 1.0) {}

		virtual bool scatter(
			const Ray &ray_in, const hit_record &record, Color &attenuation, Ray &scattered) const override {

			Vec reflected = reflect(unit_vector(ray_in.getDirection()), record.normal);
			
			scattered = Ray(record.point, reflected + fuzz * random_in_unit_sphere());
			attenuation = albedo;
			
			return (dot(scattered.getDirection(), record.normal) > 0);
		}

	public:
		Color albedo;																			// Characterizes the reflectivity of the surface of objects
		float fuzz;																				// Fuzz value
};

class Glass : public Material {
	public:
		Glass(const Color &albedo) : albedo(albedo) {}

		virtual bool scatter(
			const Ray &ray_in, const hit_record &record, Color &attenuation, Ray &scattered) const override {

			Vec reflected = reflect(unit_vector(ray_in.getDirection()), record.normal);
			
			scattered = Ray(record.point, reflected);
			attenuation = albedo;
			
			return (dot(scattered.getDirection(), record.normal) > 0);
		}

	public:
		Color albedo;																			// Characterizes the reflectivity of the surface of objects
};

class dielectric : public Material {
	public:
		dielectric(float index_of_refraction) : refraction_idx(index_of_refraction) {}

		virtual bool scatter(
			const Ray &ray_in, const hit_record &record, Color &attenuation, Ray &scattered) const override {
			// Use Snell's Law.
			attenuation = Color(1.0, 1.0, 1.0);
			float refraction_ratio = record.front_face ? (1.0 / refraction_idx) : refraction_idx;

			Vec unit_direction = unit_vector(ray_in.getDirection());
			
			float cos_theta = fmin(dot(-unit_direction, record.normal), 1.0);
			float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

			bool cannot_refract = refraction_ratio * sin_theta > 1.0;
			Vec direction;

			if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float()) {
				direction = reflect(unit_direction, record.normal);
			} else {
				direction = refract(unit_direction, record.normal, refraction_ratio);
			}

			scattered = Ray(record.point, direction);
			return true;
		}

	public:
		float refraction_idx;																	// Index of Refraction
	
	private:
		static float reflectance(double cosine, double ref_idx) {
			// Use Schlick's approximation for reflectance.
			float r0 = (1-ref_idx) / (1+ref_idx);
			r0 = r0 * r0;
			return r0 + (1 - r0) * pow((1 - cosine),5);
		}
};
