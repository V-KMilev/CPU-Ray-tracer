#pragma once

#include "Ray.h"

struct hit_record {
    float distance;                                                                                 // hit_record: distance
    Point point;                                                                                    // hit_record: hit point
    Vec normal;                                                                                     // hit_record: perperdicular to the hit point = 1
};

class hittable {
    public:
        virtual bool hit(const Ray& ray, float distance_min, float distance_max, hit_record& record) const = 0;
};