#pragma once

#include "Utility_functions.h"

class Camera {
    public:
        Camera() {

            float aspect_ratio = 16.0 / 9.0;                                                            // Image: aspect ratio
            float viewport_height = 2.0;                                                                // Camera: viewport height
            float viewport_width = aspect_ratio * viewport_height;                                      // Camera: viewport width
            float focal_length = 1.0;                                                                   // Camera: focal length: length origin to world
 
            vertical = Vec(0.0, viewport_height, 0.0);
            horizontal = Vec(viewport_width, 0.0, 0.0);
            origin = Point(0, 0, 0);
            lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec(0, 0, focal_length);
        }

        Ray get_Ray(float image_u, float image_v) const {
            return Ray(origin, lower_left_corner + image_v * vertical + image_u * horizontal - origin);
        }

    private:
        Vec vertical;                                                                                   // Camera: vertical
        Vec horizontal;                                                                                 // Camera: horizontal
        Point origin;                                                                                   // Camera: original point
        Point lower_left_corner;                                                                        // Camera: left coner
};
