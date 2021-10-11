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

const int samples_per_pixel = {100};                                        // Rays per pixel
const int max_depth = {10};                                                 // Ray bounce limit per pixel

std::vector<Color> pixels(image_height * image_width);                      // Image: output pixels

shared_ptr<Material> image_material(const char *image_name) {
	shared_ptr<Texture> image = make_shared<Image_Texture>(image_name);

	return make_shared<Lambertian>(image);
}

Hittable_list get_scene() {
	// World set: +x goes right from the camera
	// World set: +y goes up from the camera
	// World set: +z goes front from the camera
	// WOLRD right hand:
	Hittable_list world;

	// Textues
	shared_ptr<Texture> checker = make_shared<Checker_Texture>(Color(0.0, 0.0, 0.0), Color(1.0, 1.0, 1.0));
	shared_ptr<Texture> noise = make_shared<Noise_Texture>();

	// Materials
	shared_ptr<Material> material_sphere_i_0 = image_material("");

	shared_ptr<Material> material_sphere_c = make_shared<Lambertian>(checker);
	shared_ptr<Material> material_sphere_n = make_shared<Lambertian>(noise);

	shared_ptr<Material> material_white = make_shared<Lambertian>(Color(1,1,1));
	shared_ptr<Material> material_gray = make_shared<Lambertian>(Color(0.5,0.5,0.5));
	shared_ptr<Material> material_red = make_shared<Lambertian>(Color(1,0,0));
	shared_ptr<Material> material_green = make_shared<Lambertian>(Color(0,1,0));
	shared_ptr<Material> material_blue = make_shared<Lambertian>(Color(0,0,1));

	shared_ptr<Material> difflight = make_shared<Diffuse_light>(Color(11,11,6));
	shared_ptr<Material> difflight_red = make_shared<Diffuse_light>(Color(3,0,0));
	shared_ptr<Material> difflight_blue = make_shared<Diffuse_light>(Color(0,0,3));
	shared_ptr<Material> difflight_green = make_shared<Diffuse_light>(Color(0,3,0));
	shared_ptr<Material> difflight_white = make_shared<Diffuse_light>(Color(3,3,3));
	shared_ptr<Material> difflight_purple = make_shared<Diffuse_light>(Color(3,0,3));

	shared_ptr<Material> difflight_w = make_shared<Diffuse_light>(Color(7,7,7));
	shared_ptr<Material> difflight_purple_low = make_shared<Diffuse_light>(Color(0.3,0.0,0.3));
	shared_ptr<Material> difflight_white_low = make_shared<Diffuse_light>(Color(2,2,2));

	//Objects
	world.add(make_shared<xz_rect>(-1000, 1000, -1000, 1000, 0, material_sphere_n));
	// world.add(make_shared<xz_rect>(-250, 250, -250, 250, 2000, difflight_w));
	world.add(make_shared<Sphere>(Point(0, 2000, 200), 100, difflight_w));

	// l f r b from lookform(-x,+y,0)
	// l - left, f - front, r - right, b - back
	// fN - floor, wN - wall, rN - roof, p - plate
	world.add(make_shared<xy_rect>(-14, 2, 0, 10, -2, material_gray));            // l-w1
	world.add(make_shared<yz_rect>(0, 10, -2, 14, 2, material_gray));             // b-w2
	world.add(make_shared<xy_rect>(-14, 2, 0, 10, 14, material_gray));            // r-w3
	world.add(make_shared<yz_rect>(0, 10, -2, 14, -14, material_gray));           // f-w4

	world.add(make_shared<xz_rect>(-14, 2, -2, 2, 5, material_white));            // l-f1
	world.add(make_shared<xz_rect>(-14, -10, -2, 14, 5, material_white));         // f-f2
	world.add(make_shared<xz_rect>(-14, 2, 10, 14, 5, material_white));           // r-f3

	world.add(make_shared<xy_rect>(-10, 2, 0, 5, -2, material_gray));             // l-in-w1
	world.add(make_shared<yz_rect>(0, 5, -2, 10, -10, material_gray));            // f-in-w2
	world.add(make_shared<xy_rect>(-10, 2, 0, 5, 10, material_gray));             // r-in-w3

	world.add(make_shared<xz_rect>(-14, 2, -2, 2, 10, material_white));           // l-r1
	world.add(make_shared<xz_rect>(-14, -10, -2, 14, 10, material_white));        // f-r2
	world.add(make_shared<xz_rect>(-14, 2, 10, 14, 10, material_white));          // r-r3

	world.add(make_shared<xz_rect>(-8, -6, -2, 14, 10, material_white));          // p-r1
	world.add(make_shared<xz_rect>(-4, -2, -2, 14, 10, material_white));          // p-r2
	world.add(make_shared<xz_rect>(0, 2, -2, 14, 10, material_white));            // p-r3

	world.add(make_shared<xz_rect>(-4, -2, -2, 14, 5, material_white));           // f-p-f1
	// box 2x2
	world.add(make_shared<xy_rect>(-3.5, -2.5, 5, 6.5, 5.5, material_red));       // p-l-w1
	world.add(make_shared<yz_rect>(5, 6.5, 5.5, 6.5, -2.5, material_green));      // p-b-w2
	world.add(make_shared<xy_rect>(-3.5, -2.5, 5, 6.5, 6.5, material_blue));      // p-r-w3
	world.add(make_shared<yz_rect>(5, 6.5, 5.5, 6.5, -3.5, material_gray));       // p-f-w4
	world.add(make_shared<xz_rect>(-3.5, -2.5, 5.5, 6.5, 6.5, material_gray));    // p-r1

	world.add(make_shared<Sphere>(Point(-3, 7, 6), 0.5, difflight_white_low));    // light sphere

	world.add(make_shared<xz_rect>(-10, 2, 2, 10, 0, difflight_purple_low));      // light-pool

	// ????
	// world.add(make_shared<xy_rect>(5, 10, 0, 7, -11, mertial_sphere_i_0));

	return world;
}

Camera get_camera(const float aspect_ratio) {
	// CAMERA: World rules applied
	Point lookfrom(1.9, 7.0, -1.9);
	Point lookat(-5.0, 6.0, 5.0);
	Vec view_up(0.0, 1.0, 0.0);

	float dist_to_focus = 1.0;
	float aperture = 0.0;

	return Camera(lookfrom, lookat, view_up, 70.0, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
}

// WORLD:
const Hittable_list world = get_scene();

// CAMERA:
const Camera camera = get_camera(aspect_ratio);

// BACKGROUND: (0.5, 0.0, 0.5)
const Color background(0.0, 0.0, 0.0);

// 7680
// 1337