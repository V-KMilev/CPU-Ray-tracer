#pragma once

#include "Utility_functions.h"
#include "Ray.h"
#include "Vec.h"

class Camera {
	public:
		Camera(
			Point lookfrom,                                                                             // Camera: looking from
			Point lookat,                                                                               // Camera: looking at
			Vec   view_up,                                                                              // Camera: view up

			float fov,                                                                                  // Vertical field-of-view in degrees
			float aspect_ratio,                                                                         // Image: aspect ratio
			float aperture,                                                                             // Camera: aperture
			float focus_dist                                                                            // Camera: focus dist : focal length
		) {

			float degrees = degrees_to_radians(fov);                                                    // Camera: fov degrees
			float height = tan(degrees/2);                                                              // Camera: height from origin
			
			float viewport_height = 2.0 * height;                                                       // Camera: viewport height
 
			float viewport_width = aspect_ratio * viewport_height;                                      // Camera: viewport width
			
 
			w = unit_vector(lookfrom - lookat);                                                         // Camera: focal length: length origin to world
			u = unit_vector(cross(view_up, w));                                                         // Camera: vertical or the camera
			v = cross(w, u);                                                                            // Camera: horizontal of the camera

			origin  = lookfrom;
			
			horizontal = focus_dist * viewport_width * u;
			vertical = focus_dist * viewport_height * v;

			lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist * w;

			lens_radius = aperture / 2;                                                                 // Camera: light input set
		}

	   Ray get_Ray(float image_u, float image_v) const {
			
			Vec rd = lens_radius * random_in_unit_disk();                                               // Ray: get offset postion
			Vec offset = u * rd.getX() + v * rd.getY();                                                 // Ray: get cordinates of the offseted positon on the camera lens

			return Ray
			(
				origin + offset,
				lower_left_corner + image_u * horizontal + image_v * vertical - origin - offset
			);
		}

	private:
	Vec u, v, w;                                                                                        // Camera: x, y, z;
	Vec vertical;                                                                                       // Camera: vertical
	Vec horizontal;                                                                                     // Camera: horizontal
	
	Point origin;                                                                                       // Camera: original point
	Point lower_left_corner;                                                                            // Camera: lower left corner

	float lens_radius;
};
