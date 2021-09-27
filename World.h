#pragma once

#include "Utility_functions.h"

#include "Thread_manager.h"

#include "Hittable_list.h"
#include "Hittable.h"
#include "Bucket.h"

#include "Sphere_moving.h"
#include "Sphere.h"

#include "Material.h"
#include "Texture.h"
#include "AArect.h"

#include "Camera.h"

#include "Color.h"
#include "Vec.h"
#include "Ray.h" 

#define RES_TEST 800
#define RES_HD 1080
#define RES_FULL_HD 1920 
#define RES_2K 2048
#define RES_4K 3840

const float aspect_ratio = {16.0 / 9.0};                                    // Image: Aspect ratio: resolution
const int image_width = {RES_TEST};                                         // Image: Width
const int image_height = {static_cast<int>(image_width / aspect_ratio)};    // Image: Height

const int samples_per_pixel = {20};                                         // Rays per pixel
const int max_depth = {5};                                                  // Ray bounce limit per pixel

std::vector<Color> pixels(image_height * image_width);                      // Image: output pixels

// Image: 1.5 in full image width
// Image: 1 is full image heigth

shared_ptr<Material> image_material(const char *image_name) {

	shared_ptr<Texture> image = make_shared<Image_Texture>(image_name);

	return make_shared<Lambertian>(image);
}

Hittable_list random_scene() {
	// World set: +x goes right from the camera
	// World set: +y goes down from the camera
	// World set: +z goes behind from the camera
	// WOLRD left hand:
	Hittable_list world;

	// Textues
	shared_ptr<Texture> checker = make_shared<Checker_Texture>(Color(0.0, 0.0, 0.0), Color(1.0, 1.0, 1.0));
	shared_ptr<Texture> noise = make_shared<Noise_Texture>();

	// Materials
	shared_ptr<Material> mertial_sphere_i_0 = image_material("/home/vilimir/Documents/Ray-tracer/images/46180352-seamless-green-pixel-pattern-8bit-grass-lawn-texture.jpg");
	shared_ptr<Material> mertial_sphere_i_1 = image_material("/home/vilimir/Documents/Ray-tracer/images/PicsArt_09-23-10.44.28.png");

	shared_ptr<Material> material_sphere_g = make_shared<Lambertian>(checker);
	shared_ptr<Material> material_sphere_n = make_shared<Lambertian>(noise);

	shared_ptr<Material> material_sphere_m = make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
	shared_ptr<Material> material_sphere_c = make_shared<Glass>(Color(0.8, 0.8, 0.8));
	shared_ptr<Material> material_sphere_d = make_shared<Dielectric>(1.5);
	// ???
	shared_ptr<Material> difflight = make_shared<Diffuse_light>(Color(4,4,4));

	//Objects
	world.add(make_shared<Sphere>(Point(0.0, 1000.5, 0.0), 1000.0, material_sphere_g));
	world.add(make_shared<Sphere>(Point(0,-2,0), 2, material_sphere_n));
	// ????
	// world.add(make_shared<xy_rect>(3, 5, -1, -3, 2, difflight));

	return world;
}

Camera get_camera(const float aspect_ratio) {

	// CAMERA: World rules applied
	// ???? ?
	Point lookfrom(+27.0, -7.0, +7.0);
	Point lookat(-0.0, -2.0, -10.0);
	Vec view_up(+0.0, +1.0, +0.0);    // Its +1 because of the camera's coordinate not the world's

	float dist_to_focus = lookat.getZ() < 0 ? lookat.getZ() * (-1) : lookat.getZ();
	float aperture = 0.1;

	return Camera(lookfrom, lookat, view_up, 37.0, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
}

// WORLD:
const Hittable_list world = random_scene();

// CAMERA:
const Camera camera = get_camera(aspect_ratio);

// BACKGROUND:
const Color background(1,1,0);

// 7680
// 1337