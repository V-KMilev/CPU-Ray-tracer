#pragma once

#include "Utility_functions.h"
#include "Ray.h"
#include "Vec.h"

struct hit_record;

class Material {                                                                        // Abstract Class
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
        Color albedo;                                                                   // Characterizes the reflectivity of the surface of objects

};

class Metal : public Material {
    public:
        Metal(const Color &albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        virtual bool scatter(
            const Ray &ray_in, const hit_record &record, Color &attenuation, Ray &scattered) const override {

            Vec reflected = reflect(unit_vector(ray_in.getDirection()), record.normal);
            
            scattered = Ray(record.point, reflected + fuzz * random_in_unit_sphere());
            attenuation = albedo;
            
            return (dot(scattered.getDirection(), record.normal) > 0);
        }

    public:
        Color albedo;                                                                   // Characterizes the reflectivity of the surface of objects
        float fuzz;                                                                     // Fuzz value
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
        Color albedo;                                                                   // Characterizes the reflectivity of the surface of objects
};