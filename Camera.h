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
			float focus_dist,                                                                           // Camera: focus dist : focal length

			float s_time ,                                                                              // Camera; start time
			float e_time                                                                                // Camera: end time
		) {

			float degrees = degrees_to_radians(fov);                                                    // Camera: fov degrees
			float width = tan(degrees/2);                                                               // Camera: width from origin side01 side02
			
			float viewport_height = 2.0 * width;                                                        // Camera: viewport height
			float viewport_width = aspect_ratio * viewport_height;                                      // Camera: viewport width
			
			// Camera plate
			w = unit_vector(lookfrom - lookat);                                                         // Camera: focal length: length origin to world
			u = unit_vector(cross(view_up, w));                                                         // Camera: vertical or the camera
			v = cross(w, u);                                                                            // Camera: horizontal of the camera

			origin  = lookfrom;
			
			// Low left corner math
			horizontal = focus_dist * viewport_width * u;
			vertical = focus_dist * viewport_height * v;
			lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist * w;

			lens_radius = aperture / 2;                                                                 // Camera: light input set

			this-> s_time = s_time;
			this-> e_time = e_time;
		}

		Ray get_ray(float image_u, float image_v) const {
			
			Vec rd = lens_radius * random_in_unit_disk();                                               // Ray: get offset postion
			Vec offset = u * rd.getX() + v * rd.getY();                                                 // Ray: get cordinates of the offseted positon on the camera lens

			return Ray
			(
				origin + offset,
				lower_left_corner + image_u * horizontal + image_v * vertical - origin - offset,
				random_float(s_time, e_time)
			);
		}

	private:
		Vec u, v, w;                                                                                        // Camera: x, y, z;
		Vec vertical;                                                                                       // Camera: vertical
		Vec horizontal;                                                                                     // Camera: horizontal
		
		Point origin;                                                                                       // Camera: original point
		Point lower_left_corner;                                                                            // Camera: lower left corner

		float lens_radius;                                                                                  // Camera: lens radius
		float s_time;                                                                                       // Camera: open time
		float e_time;                                                                                       // Camera: close time
};
