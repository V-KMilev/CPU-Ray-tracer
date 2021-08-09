#pragma once

#include "Utility_functions.h"
#include "Ray.h"
#include "Vec.h"

class Camera {
    public:
        Camera(
            Point lookfrom,                                                                             // Camera: looking from
            Point lookat,                                                                               // Camera: looking at
            Vec view_up,                                                                                // Camera: view up
            float vfov,                                                                                 // Vertical field-of-view in degrees
            float aspect_ratio                                                                          // Image: aspect ratio
        ) {


            float degrees = degrees_to_radians(vfov);                                                   // Camera: fov degrees
            float height = tan(degrees/2);                                                              // Camera: height from origin
            
            float viewport_height = 2.0 * height;                                                       // Camera: viewport height
            float viewport_width = aspect_ratio * viewport_height;                                      // Camera: viewport width
            
            float focal_length = 1.0;                                                                   // Camera: focal length: length origin to world
 
            Vec w = unit_vector(lookfrom - lookat);                                                     // Camera:
            Vec u = unit_vector(cross(view_up, w));                                                     // Camera:
            Vec v = cross(w, u);                                                                        // Camera:

            origin = lookfrom;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - w;

        }

        Ray get_Ray(float image_u, float image_v) const {
            return Ray(origin, lower_left_corner + image_u * horizontal + image_v * vertical - origin);
        }

    private:
        Vec vertical;                                                                                   // Camera: vertical
        Vec horizontal;                                                                                 // Camera: horizontal
        Point origin;                                                                                   // Camera: original point
        Point lower_left_corner;                                                                        // Camera: lower left corner
};
