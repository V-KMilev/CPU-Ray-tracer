#pragma once

#include "Utility_functions.h"

#include "Texture.h"

#include "Color.h"
#include "Ray.h"
#include "Vec.h"

/* Forward declaration */
struct hit_record;

enum Material_ID: unsigned int {
	t_lambertian    = 1,
	t_diffuse_light = 2
};

class Material {    // Interface
	public:

		/*
		 * Function - scatter
		 *
		 * Parameters:
		 * [p] const Ray &ray_in,
		 * [p] const hit_record &record,
		 * [p] Color &attenuation,
		 * [p] Ray &scattered
		 * 
		 * Return type:
		 * bool
		 * 
		 * Use:
		 * Base funtion (virtual). 
		 * Scatter is used to understand (to simulate) 
		 * the way the Rays are scattering.
		 */

		virtual bool scatter(
			const Ray &ray_in, const hit_record &record, Color &attenuation, Ray &scattered
		) const = 0;

		/*
		 * Function - emitted
		 *
		 * Parameters:
		 * [p] float u,
		 * [p] float v,
		 * [p] const Point &point
		 * 
		 * Return type:
		 * Color
		 * 
		 * Use:
		 * Base funtion (virtual). 
		 * Emitted is used only for light sources. 
		 */

		virtual Color emitted(float u, float v, const Point &point) const {
			return Color(0,0,0);
		}

		public:
			const char* my_name;
			Material_ID id;
};

class Lambertian : public Material {
	public:
		Lambertian(const Color &albedo) : albedo(make_shared<Solid_Color>(albedo)) {
			my_name = "Lambertian";
			id = Material_ID::t_lambertian;
		}
		Lambertian(shared_ptr<Texture> albedo) : albedo(albedo) {
			my_name = "Lambertian";
			id = Material_ID::t_lambertian;
		}

		/*
		 * Function - scatter
		 *
		 * Use:
		 * Override of base funtion.
		 * Lambertian scatter is aways true. 
		 * In this case scatter updates the 
		 * attenuation Color and the scattered Ray.
		 * 
		 * [p] Attenuation Color - represents the Color value of the albedo
		 * [p] Scattered Ray - Ray(hit_record.point, hit_record.normal + 
		 * random vector , ...).
		 */

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

class Diffuse_Light : public Material  {
	public:
		Diffuse_Light(shared_ptr<Texture> texture) : emit(texture) {
			my_name = "Diffuse-light";
			id = Material_ID::t_diffuse_light;
		}
		Diffuse_Light(Color color) : emit(make_shared<Solid_Color>(color)) {
			my_name = "Diffuse-light";
			id = Material_ID::t_diffuse_light;
		}

		/*
		 * Function - scatter
		 *
		 * Use:
		 * Override of base funtion.
		 * Diffuse_light scatter is aways false. 
		 * Rays can't scatter in light so...
		 */

		virtual bool scatter(
			const Ray &ray_in, const hit_record &record, Color &attenuation, Ray &scattered) const override {
			return false;
		}

		/*
		 * Function - emitted
		 *
		 * Use:
		 * Override of base funtion.
		 * Return the Color value of the emit.
		 */

		virtual Color emitted(float u, float v, const Point &point) const override {    //emitted
			return emit->value(u, v, point);
		}

	public:
		shared_ptr<Texture> emit;
};
