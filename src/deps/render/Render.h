#pragma once

#include "Settings.h"
#include "World.h"
#include "Log.h"

/* WORLD: */
const Hittable_list world = get_scene();

/*
 * Function - tracer
 *
 * Parameters:
 * [p] const Ray &ray,
 * [p] const Color &background,
 * [p] const Hittable &world,
 * [p] int depth
 * 
 * Return type:
 * Color
 * 
 * Use:
 * By calling tracer you get 
 * in return the calculated color 
 * value of a pixel of the scene.
 * 
 * The simulation that tracer does is 
 * it traces the ray (ray-tracer be like).
 */

Color tracer(const Ray &ray, const Color &background, const Hittable &world, int depth) {

	hit_record record;
	/* If we've exceeded the ray bounce limit, no more light is gathered. */
	if (depth <= 0) {
		return Color(0, 0, 0);    // Set current pixel value to black
	}

	/* If the ray hits nothing, return the background Color. */
	/* 0.001 to fix the shadow problem */
	if (!world.hit(ray, 0.001, infinity, record)) {
		return background;
	}

	Ray scattered;
	Color attenuation;
	Color emitted = record.material_ptr->emitted(record.u, record.v, record.point);

	/* If the material we hit is not scatterable, return emitted value. */
	/* (If we hit lightsource return its Color value.) */
	if (!record.material_ptr->scatter(ray, record, attenuation, scattered)) {
		return emitted;
	}

	/* If we hit non lightsource-able object return object materials value. */
	return emitted + attenuation * tracer(scattered, background, world, depth-1);
}

/*
 * Function - render
 *
 * Parameters:
 * [p] const Bucket &my_bucket
 * 
 * Return type:
 * void
 * 
 * Use:
 * By calling render you find pixel from 
 * the image to be calculated and 
 * to set its color value. 
 * Updates (fills) the pixels vector.
 */

void render(const Bucket &my_bucket) {
	/* CAMERA: */
	Camera camera(lookfrom, lookat, view_up, 53.7, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

	Logger::getDefaultLogger() << "\n\rStart Bucket: " << std::this_thread::get_id() << " -> + " << my_bucket.bucket_id;

	#ifdef DEBUG
		std::cerr << "\n\rStart Bucket: " << std::this_thread::get_id() << " -> + " << my_bucket.bucket_id;
	#endif

	for (int y = my_bucket.start_y; y < my_bucket.end_y; y++) {
		for (int x = my_bucket.start_x; x < my_bucket.end_x; x++) {

			/* Current index in the full image. */
			const int idx = image_width * y + x;

			/* Total samples in the current pixel */
			const int total = samples_in_pixels[idx] + samples_per_pixel;

			/* Previous current pixel */
			/* We take is as reference so we can override it at the end. */
			Color &old_pixel = pixels[idx];

			/* Current pixel */
			Color current_pixel(0, 0, 0);

			for (int s = 0; s < samples_per_pixel; s++) {

				/* uv random value */
				float u = (x + random_float()) / (image_width - 1);
				float v = (y + random_float()) / (image_height - 1);

				Ray ray = camera.get_ray(u, v);

				/* Set current pixel Color value. An avg. of all samples on it. */
				current_pixel += tracer(ray, background, world, max_depth);
			}

			/* 1.0f to converted to float */
			/* We divide it by the samples so we can fix the value */
			current_pixel *= (1.0f / samples_per_pixel);

			/* Override of the pixel in the matrix */
			/* Current index is equal to the fixed value of the sum of the old and new pixel */
			old_pixel = old_pixel * (samples_in_pixels[idx] / (float) total) +  current_pixel * (samples_per_pixel / (float) total);

			/* Update the samples set in this index */
			samples_in_pixels[idx] = total;
		}
	}

	/* Update samples if the image is finished */
	if(++samples_in_counter == total_buckets) {

		if(!change_static) {
			samples_per_pixel += 2;
		}
		samples_in_counter.store(0);
	}

	Logger::getDefaultLogger() << "\n\rEnd   Bucket: " << std::this_thread::get_id() << " -> - " << my_bucket.bucket_id;

	#ifdef DEBUG
		std::cerr << "\n\rEnd   Bucket: " << std::this_thread::get_id() << " -> - " << my_bucket.bucket_id;
	#endif
}