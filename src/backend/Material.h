#pragma once

#include "Utility_functions.h"

#include "Texture.h"

#include "Ray.h"
#include "Vec.h"
#include "Color.h"

struct hit_record;

class Material {    // Abstract Class
	public:
		virtual bool scatter(
			const Ray &ray_in, const hit_record &record, Color &attenuation, Ray &scattered
		) const = 0;

		virtual Color emitted(float u, float v, const Point &point) const {
			return Color(0,0,0);
		}
};

class Lambertian : public Material {
	public:
		Lambertian(const Color &albedo) : albedo(make_shared<Solid_Color>(albedo)) {}
		Lambertian(shared_ptr<Texture> albedo) : albedo(albedo) {}

		virtual bool scatter(
			const Ray &ray_in, const hit_record &record, Color &attenuation, Ray &scattered) const override {
			
			Vec scatter_direction = record.normal + random_unit_vector();

			/* Catch degenerate scatter direction */
			if (scatter_direction.near_zero()) {
				scatter_direction = record.normal;
			}

			scattered = Ray(record.point, scatter_direction, ray_in.get_time());
			attenuation = albedo->value(record.u, record.v, record.point);

			return true;
		}

	public:
		shared_ptr<Texture> albedo;    // Characterizes the reflectivity of the surface of objects
};

class Diffuse_light : public Material  {
	public:
		Diffuse_light(shared_ptr<Texture> texture) : emit(texture) {}
		Diffuse_light(Color color) : emit(make_shared<Solid_Color>(color)) {}

		virtual bool scatter(
			const Ray &ray_in, const hit_record &record, Color &attenuation, Ray &scattered) const override {
			return false;
		}

		virtual Color emitted(float u, float v, const Point &point) const override {    //emitted
			return emit->value(u, v, point);
		}

	public:
		shared_ptr<Texture> emit;
};
