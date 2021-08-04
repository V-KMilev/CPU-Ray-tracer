#pragma once

#include "Hittable.h"
#include "Vec.h"

class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(Point center, float radius) : center(center), radius(radius) {};

        virtual bool hit(
            const Ray &ray, float distance_min, float distance_max, hit_record &record) const override;

    public:
        Point center;                                                                                           // Sphere: center
        float radius;                                                                                           // Sphere: radius
};

bool Sphere::hit(const Ray &ray, float distance_min, float distance_max, hit_record &record) const {

    Vec oc = ray.getOrigin() - center;                                                                          // Vec OC: camera to center
    
    float a = ray.getDirection().length_squared();                                                              // Discriminant: a: ray.dir * sqrt(length)
    float half_b = dot(oc, ray.getDirection());                                                                 // Discriminant: half b:  
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

    Vec outward_normal = record.normal;
    record.set_face_normal(ray, outward_normal);

    return true;
}