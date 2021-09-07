#include <iostream>
#include <fstream>
#include <ctime>

#include "World.h"

Color ray_color(const Ray &ray, const Hittable &world, int depth) {

	hit_record record;
	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0) {
		return Color(0, 0, 0);                                                          // Set current pixel value to black
	}

	if (world.hit(ray, 0.001, infinity, record)) {                                      // 0.001 to fix shadow problem

		Ray scattered;
		Color attenuation;

		if (record.material_ptr->scatter(ray, record, attenuation, scattered)) {
			return attenuation * ray_color(scattered, world, depth - 1);
		}

		return Color(0, 0, 0);
	}

	Vec unit_direction = unit_vector(ray.getDirection());
	float distance = 0.5 * (unit_direction.getY() + 1.0);

	return (1.0 - distance) * Color(0.5, 0.0, 1.0) + distance * Color(0.5, 0.7, 1.0);   // Blend Value
}

void render(const Bucket &my_bucket) {

	std::cerr << "\nStart Bucket: " << my_bucket.bucket_id;

	for (int y = my_bucket.start_y; y < my_bucket.end_y; y++) {
		for (int x = my_bucket.start_x; x < my_bucket.end_x; x++) {

			Color pixel_color(0, 0, 0);

			for (int s = 0; s < samples_per_pixel; s++) {

				float u = (x + random_float()) / (image_width - 1);
				float v = (y + random_float()) / (image_height - 1);

				Ray ray = camera.get_Ray(u, v);

				pixel_color += ray_color(ray, world, max_depth);
			}
			pixels[image_width * y + x] = color_gama(pixel_color, samples_per_pixel);
		}
	}
	std::cerr << "\nEnd   Bucket: " << my_bucket.bucket_id;
}

void file_write(std::ofstream &out, std::vector<Color> pixels, const int image_width, const int image_height) {

	out << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int idx = 0; idx < pixels.size(); idx++) {
		out << static_cast<int>(256 * clamp(pixels[idx].getX(), 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(pixels[idx].getY(), 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(pixels[idx].getZ(), 0.0, 0.999)) << '\n';
	}

	out.close();
}

int main(int argc, char **argv) {

	{
		time_t now_s = time(0);
		// convert now to string form
		char *dt = ctime(&now_s);

		std::cerr << "\rStart time: " << dt;
	}

	// OUT STREAM:
	std::ofstream out("RTout.ppm");

	// RENDER:
	ThreadPool pool(MAX_NUMBER_OF_THREADS);

	std::vector<Bucket> my_buckets = bucket_segmentation(image_width, image_height);

	for(Bucket &my_bucket : my_buckets) {

		pool.enter_queue(my_bucket);
	}
	pool.master_wait();

	// FILE WRITE:
	file_write(out, pixels, image_width, image_height);

	{
		time_t now_e = time(0);
		// convert now to string form
		char *dt = ctime(&now_e);

		std::cerr << "\n\rEnd time: " << dt;
	}

	return 0;
}
