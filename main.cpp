#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>

#include "Utility_functions.h"

#include "Thread_manager.h"

#include "Hittable_list.h"
#include "Hittable.h"
#include "Bucket.h"

#include "Material.h"
#include "Sphere.h"

#include "Camera.h"

#include "Color.h"
#include "Vec.h"
#include "Ray.h"

Hittable_list random_scene() {
	// World set: +x goes right from the camera
	// World set: +y goes down from the camera
	// World set: +z goes behind from the camera
	// WOLRD left hand:
	Hittable_list world;

	shared_ptr<Material> material_sphere_g = make_shared<Lambertian>(Color(0.1, 0.1, 0.1));
	shared_ptr<Material> material_sphere_c = make_shared<Glass>(Color(0.8, 0.8, 0.8));
	shared_ptr<Material> material_sphere_m = make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
	shared_ptr<Material> material_sphere_d = make_shared<dielectric>(1.5);

	for (int i = -5; i < 5; i++) {
		for (int j = -5; j < 5; j++) {

			shared_ptr<Material> material_sphere_l = make_shared<Lambertian>(Color(random_float(0, 1), random_float(0, 1), random_float(0, 1)));

			Point position(j * 10 * random_float(), 0.0, i * 10 * random_float());

			if (i % 3 == 0 || i % 7 == 0)
			{
				world.add(make_shared<Sphere>(
					position,
					random_float(0.5, 1.0),
					material_sphere_l));
			}
			else if (i % 4 == 0)
			{
				world.add(make_shared<Sphere>(
					position,
					random_float(0.5, 1.0),
					material_sphere_m));
			}
			else if (i % 5 == 0)
			{
				world.add(make_shared<Sphere>(
					position,
					random_float(0.5, 1.0),
					material_sphere_d));
			}
			else
			{
				world.add(make_shared<Sphere>(
					position,
					random_float(0.5, 1.0),
					material_sphere_c));
			}
		}
	}

	world.add(make_shared<Sphere>(Point(-10.0, -3.5, -17.0), 3.5, material_sphere_c));
	world.add(make_shared<Sphere>(Point(-0.0, -3.5, -20.0), 3.5, material_sphere_m));
	world.add(make_shared<Sphere>(Point(+10.0, -3.5, -17.0), 3.5, material_sphere_d));

	world.add(make_shared<Sphere>(Point(0.0, 1000.5, -1.0), 1000.0, material_sphere_g));

	return world;
}

Color ray_color(const Ray &ray, const Hittable &world, int depth) {

	hit_record record;
	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0) {
		return Color(0, 0, 0);                                                                 // Set current pixel value to black
	}

	if (world.hit(ray, 0.001, infinity, record)) { // 0.001 to fix shadow problem

		Ray scattered;
		Color attenuation;

		if (record.material_ptr->scatter(ray, record, attenuation, scattered)) {
			return attenuation * ray_color(scattered, world, depth - 1);
		}

		return Color(0, 0, 0);
	}

	Vec unit_direction = unit_vector(ray.getDirection());
	float distance = 0.5 * (unit_direction.getY() + 1.0);

	return (1.0 - distance) * Color(1.0, 0.0, 0.0) + distance * Color(0.5, 0.7, 1.0);          // Blend Value
}

int main(int argc, char** argv) {

	time_t now_s = time(0);

	// convert now to string form
	char *dt = ctime(&now_s);

	std::cerr << "\rStart time: " << dt;

	// Image: 1.5 in full image width
	// Image: 1 is full image heigth
	const float aspect_ratio = { 16.0 / 9.0 };                                                  // Image: Aspect ratio: resolution
	const int image_width = { 300 };                                                            // Image: Width
	const int image_height = { static_cast<int>(image_width / aspect_ratio) };                  // Image: Height

	const int samples_per_pixel= { 100 };                                                       // Rays per pixel
	const int max_depth = { 2 };                                                                // Ray bounce limit per pixel

	// WORLD:
	Hittable_list world = random_scene();

	// CAMERA: World rules applied
	Point lookfrom(+20.0, -5.0, +10.0);
	Point lookat(-0.0, -4.5, -17.0);
	Vec view_up(+0.0, +1.0, +0.0);                                                              // Its +1 because of the camera's coordinate not the world's

	float dist_to_focus = lookat.getZ() < 0 ? lookat.getZ() * (-1) : lookat.getZ();
	float aperture = 0.1;

	Camera camera(lookfrom, lookat, view_up, 37.0, aspect_ratio, aperture, dist_to_focus);

	// OUT STREAM:
	std::ofstream out("RTout.ppm");

	// RENDER:
	std::cerr << "Rendering...\n";

	std::vector<Bucket> buckets;

	buckets = bucket_segmentation(image_width, image_height);

	std::vector<Color> pixels(image_height * image_width);
	
	//THREAD MANAGER:
	ThreadPool pool(pool.MAX_NUMBER_OF_THREADS);

	auto work = pool.enter_queue([]() {});

	for (int idx = 0; idx < buckets.size(); idx++) {

		Bucket bucket = buckets[idx];

		for (int y = bucket.start_y; y < bucket.end_y; y++) {
			for (int x = bucket.start_x; x < bucket.end_x; x++) {

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
	}

	// FILE WRITE:
	out << "P3\n"
		<< image_width << " " << image_height << "\n255\n";

	for (int idx = 0; idx < pixels.size(); idx++) {
		out << static_cast<int>(256 * clamp(pixels[idx].getX(), 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(pixels[idx].getY(), 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(pixels[idx].getZ(), 0.0, 0.999)) << '\n';
	}
	out.close();

	time_t now_e = time(0);

	// convert now to string form
	dt = ctime(&now_e);

	std::cerr << "\n\rEnd time: " << dt;
	
	return 0;
}
// 7680
// 1337