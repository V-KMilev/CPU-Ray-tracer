#pragma once

#include "Ray.h"

struct hit_record {
    float distance;                                                                                 // hit_record: distance
    Point point;                                                                                    // hit_record: hit point
    Vec normal;                                                                                     // hit_record: parale
    
    bool front_face;                                                                                // hit_record: parallel to the tangent

    inline void set_face_normal(const Ray& r, const Vec& outward_normal) {
        
        front_face = dot(r.getDirection(), outward_normal) < 0;                                     // hit_record: if is in the sphere
        normal = front_face ? outward_normal : -outward_normal;                                     // hit_record: set it to go in the sphere
    }
};

class Hittable {
    public:
        virtual bool hit(const Ray& ray, float distance_min, float distance_max, hit_record& record) const = 0;
};