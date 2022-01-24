#pragma once

#include "World.h"
#include "Settings.h"

/* WORLD: */
const Hittable_list world = get_scene();

Color tracer(const Ray &ray, const Color &background, const Hittable &world, int depth) {

	hit_record record;
	/* If we've exceeded the ray bounce limit, no more light is gathered. */
	if (depth <= 0) {
		return Color(0, 0, 0);    // Set current pixel value to black
	}

	/* If the ray hits nothing, return the background color. */
	/* 0.001 to fix the shadow problem */
	if (!world.hit(ray, 0.001, infinity, record)) {
		return background;
	}

	Ray scattered;
	Color attenuation;

	Color emitted = record.material_ptr->emitted(record.u, record.v, record.point);

	if (!record.material_ptr->scatter(ray, record, attenuation, scattered)) {
		return emitted;
	}

	return emitted + attenuation * tracer(scattered, background, world, depth-1);
}

void render(const Bucket &my_bucket) {
	/* CAMERA: */
	Camera camera(lookfrom, lookat, view_up, 53.7, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

	const float div = 1.0f / samples_per_pixel;

	std::cerr << "\n\rStart Bucket: " << std::this_thread::get_id() << " -> + " << my_bucket.bucket_id;

	for (int y = my_bucket.start_y; y < my_bucket.end_y; y++) {
		for (int x = my_bucket.start_x; x < my_bucket.end_x; x++) {

			const int idx = image_width * y + x;
			const int total = samples_in_pixels[idx] + samples_per_pixel;

			Color pixel_in_set(0, 0, 0);
			Color &current_pixel = pixels[idx];

			for (int s = 0; s < samples_per_pixel; s++) {

				float u = (x + random_float()) / (image_width - 1);
				float v = (y + random_float()) / (image_height - 1);

				Ray ray = camera.get_ray(u, v);

				pixel_in_set += tracer(ray, background, world, max_depth);
			}
			pixel_in_set *= div;

			current_pixel = current_pixel * (samples_in_pixels[idx] / (float) total) +  pixel_in_set * (samples_per_pixel / (float) total);

			samples_in_pixels[idx] = total;
		}
	}

	if(++samples_in_counter == total_buckets) {
		samples_per_pixel += 2;
		samples_in_counter.store(0);
	}

	std::cerr << "\n\rEnd   Bucket: " << std::this_thread::get_id() << " -> - " << my_bucket.bucket_id;
}
