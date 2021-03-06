#pragma once

#include "Utility_functions.h"
#include "Ray.h"
#include "Vec.h"

class Camera {
	public:
		Camera(
			Point lookfrom,        // Camera: looking from
			Point lookat,          // Camera: looking at
			Vec   view_up,         // World: view up

			float fov,             // Vertical field-of-view in degrees
			float aspect_ratio,    // Image: aspect ratio
			float aperture,        // Camera: aperture
			float focus_dist,      // Camera: focus dist : focal length

			float s_time,          // Camera; start time
			float e_time           // Camera: end time
		) {

			float degrees = degrees_to_radians(fov);                                        // fov degrees
			float width = tan(degrees/2);                                                   // width from origin side01 side02
			
			float viewport_height = 2.0 * width;                                            // viewport height
			float viewport_width = aspect_ratio * viewport_height;                          // viewport width
			
			/* Camera plate */
			front = unit_vector(lookat - lookfrom);                                         // depth (front) of the camera
			right = unit_vector(cross(view_up, front));                                     // horizontal (right) of the camera
			real_up = cross(right, front);                                                  // vertical (up) of the camera

			origin = lookfrom;

			/* Lower left corner math */
			horizontal = focus_dist * viewport_width * right;
			vertical = focus_dist * viewport_height * real_up;
			lower_left_corner = origin - horizontal/2 - vertical/2 + focus_dist * front;

			lens_radius = aperture / 2;                                                     // light input set

			this-> s_time = s_time;
			this-> e_time = e_time;
		}

		/*
		 * Function - get_ray
		 *
		 * Parameters:
		 * [p] float image_u,
		 * [p] float image_v
		 * 
		 * Return type:
		 * Ray
		 * 
		 * Use:
		 * By calling get_ray you get 
		 * in return random Ray from camera.
		 */

		Ray get_ray(float image_u, float image_v) const {

			Vec rd = lens_radius * random_in_unit_disk();            // Ray: offset postion
			Vec offset = right * rd.getX() + real_up * rd.getY();    // Ray: cordinates of the offseted positon on the camera lens

			return Ray
			(
				origin + offset,
				lower_left_corner + image_u * horizontal + image_v * vertical - origin - offset,
				random_float(s_time, e_time)
			);
		}

	private:
		Vec right, real_up, front;    // Camera: x, y, z;
		Vec vertical;                 // Camera: vertical
		Vec horizontal;               // Camera: horizontal

		Point origin;                 // Camera: original point
		Point lower_left_corner;      // Camera: lower left corner

		float lens_radius;            // Camera: lens radius
		float s_time;                 // Camera: open time
		float e_time;                 // Camera: close time
};
