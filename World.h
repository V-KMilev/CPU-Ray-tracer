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

const int samples_per_pixel = {50};                                         // Rays per pixel
const int max_depth = {5};                                                  // Ray bounce limit per pixel

std::vector<Color> pixels(image_height * image_width);                      // Image: output pixels

shared_ptr<Material> image_material(const char *image_name) {
	shared_ptr<Texture> image = make_shared<Image_Texture>(image_name);

	return make_shared<Lambertian>(image);
}

Hittable_list random_scene() {
	// World set: +x goes right from the camera
	// World set: +y goes up from the camera
	// World set: +z goes front from the camera
	// WOLRD right hand:
	Hittable_list world;

	// Textues
	shared_ptr<Texture> checker = make_shared<Checker_Texture>(Color(0.0, 0.0, 0.0), Color(1.0, 1.0, 1.0));
	shared_ptr<Texture> noise = make_shared<Noise_Texture>();

	// Materials
	shared_ptr<Material> mertial_sphere_i_0 = image_material("");

	shared_ptr<Material> material_sphere_c = make_shared<Lambertian>(checker);
	shared_ptr<Material> material_sphere_n = make_shared<Lambertian>(noise);

	shared_ptr<Material> difflight = make_shared<Diffuse_light>(Color(11,11,6));
	shared_ptr<Material> difflight_red = make_shared<Diffuse_light>(Color(3,0,0));
	shared_ptr<Material> difflight_blue = make_shared<Diffuse_light>(Color(0,0,3));
	shared_ptr<Material> difflight_green = make_shared<Diffuse_light>(Color(0,3,0));
	shared_ptr<Material> difflight_white = make_shared<Diffuse_light>(Color(3,3,3));
	shared_ptr<Material> difflight_purple = make_shared<Diffuse_light>(Color(3,0,3));

	//Objects
	world.add(make_shared<xz_rect>(-1000, 1000, -1000, 1000, 0, material_sphere_n));
	world.add(make_shared<xz_rect>(-250, 250, -250, 250, 2000, difflight));

	world.add(make_shared<Sphere>(Point(0, 2, 0), 2, material_sphere_c));
	world.add(make_shared<xy_rect>(5, 10, 0, 7, -11, mertial_sphere_i_0));

	world.add(make_shared<Sphere>(Point(25, 2, 0), 2, difflight_blue));
	world.add(make_shared<Sphere>(Point(11, 2, -23), 2, difflight_red));
	world.add(make_shared<xy_rect>(15, 20, 0, 7, -7, difflight_purple));
	world.add(make_shared<xy_rect>(-8, -5, 0, 5, -3, difflight_green));
	world.add(make_shared<xy_rect>(-2, 2, 0, 4, 3, difflight_white));

	return world;
}

Camera get_camera(const float aspect_ratio) {
	// CAMERA: World rules applied
	Point lookfrom(-15.0, 7.0, 13.0);
	Point lookat(0.0, 2.0, 0.0);
	Vec view_up(0.0, 1.0, 0.0);

	float dist_to_focus = 1.0;
	float aperture = 0.0;

	return Camera(lookfrom, lookat, view_up, 37.0, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
}

// WORLD:
const Hittable_list world = random_scene();

// CAMERA:
const Camera camera = get_camera(aspect_ratio);

// BACKGROUND: (0.5, 0.0, 0.5)
const Color background(0.0, 0.0, 0.0);

// 7680
// 1337