#pragma once

#include "World.h"
#include "Settings.h"

/* WORLD: */
const Hittable_list world = get_scene();

/* CAMERA: */
Camera camera(lookfrom, lookat, view_up, 53.7, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

Color ray_color(const Ray &ray, const Color &background, const Hittable &world, int depth) {

	hit_record record;
	/* If we've exceeded the ray bounce limit, no more light is gathered. */
	if (depth <= 0) {
		return Color(0, 0, 0);    // Set current pixel value to black
	}

	/* If the ray hits nothing, return the background color. */
	if (!world.hit(ray, 0.001, infinity, record)) {    // 0.001 to fix the shadow problem
		return background;
	}

	Ray scattered;
	Color attenuation;
	Color emitted = record.material_ptr->emitted(record.u, record.v, record.point);

	if (!record.material_ptr->scatter(ray, record, attenuation, scattered)) {
		return emitted;
	}

	return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}

void render(const Bucket &my_bucket) {

	std::cerr << "\n\rStart Bucket: " << std::this_thread::get_id() << " -> + " << my_bucket.bucket_id;

	for (int y = my_bucket.start_y; y < my_bucket.end_y; y++) {
		for (int x = my_bucket.start_x; x < my_bucket.end_x; x++) {

			Color pixel_color(0, 0, 0);

			for (int s = 0; s < samples_per_pixel; s++) {

				float u = (x + random_float()) / (image_width - 1);
				float v = (y + random_float()) / (image_height - 1);

				Ray ray = camera.get_ray(u, v);

				pixel_color += ray_color(ray, background, world, max_depth);
			}

			const int idx = image_width * y + x;
			Color &c = pixels[idx];
			const int total = samples_per_pixel + spp[idx];
			
			c = c * (spp[idx] / float(total)) +  pixel_color * (samples_per_pixel / float(total));
			spp[idx] = total;
		}
	}

	std::cerr << "\n\rEnd   Bucket: " << std::this_thread::get_id() << " -> - " << my_bucket.bucket_id;
}
