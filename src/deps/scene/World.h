#pragma once

#include "Utility_functions.h"

#include "Color.h"
#include "Vec.h"
#include "Ray.h"

#include "Camera.h"
#include "Bucket.h"

#include "Hittable_list.h"
#include "Hittable.h"

#include "Material.h"
#include "Texture.h"

#include "Sphere_moving.h"
#include "Sphere.h"
#include "AArect.h"
#include "Obj.h"
#include "Box.h"

/*
 * Function - image_material
 *
 * Parameters:
 * [p] const char *image_name
 * 
 * Return type:
 * shared_ptr<Material>
 * 
 * Use:
 * By calling image_material you get 
 * in return the image you have set as 
 * a Material.
 */

shared_ptr<Material> image_material(const char *image_name) {
	shared_ptr<Texture> image = make_shared<Image_Texture>(image_name);

	return make_shared<Lambertian>(image);
}

/*
 * Function - get_scene
 *
 * Parameters:
 * none
 * 
 * Return type:
 * Hittable_list
 * 
 * Use:
 * By calling get_scene you get 
 * in return the hardcoded scene you 
 * have created (written).
 * 
 * The scene works with the right hand rule 
 * for the coordinates.
 */

Hittable_list get_scene() {
	Hittable_list world;

////////////////////////////////////////////////////////////////////////////

	/* Textues */
	shared_ptr<Texture> checker_w = make_shared<Checker_Texture>(Color(0.0, 0.0, 0.0), Color(1.0, 1.0, 1.0));
	shared_ptr<Texture> checker_m = make_shared<Checker_Texture>(Color(0.0, 0.0, 0.0), Color(1.0, 0.0, 1.0));
	shared_ptr<Texture> checker_r = make_shared<Checker_Texture>(Color(0.0, 0.0, 0.0), Color(1.0, 0.0, 0.0));

	shared_ptr<Texture> random_img = make_shared<Image_Texture>("sdfdsfgdfg.png");

////////////////////////////////////////////////////////////////////////////

	/* Materials */
	shared_ptr<Material> material_white_c = make_shared<Lambertian>(checker_w);
	shared_ptr<Material> material_magenta_c = make_shared<Lambertian>(checker_m);
	shared_ptr<Material> material_red_c = make_shared<Lambertian>(checker_r);

	shared_ptr<Material> random_img_m = make_shared<Lambertian>(random_img);

	shared_ptr<Material> material_white = make_shared<Lambertian>(Color(1,1,1));
	shared_ptr<Material> material_black = make_shared<Lambertian>(Color(0,0,0));
	shared_ptr<Material> material_green = make_shared<Lambertian>(Color(0,1,0));
	shared_ptr<Material> material_blue  = make_shared<Lambertian>(Color(0,0,1));
	shared_ptr<Material> material_red   = make_shared<Lambertian>(Color(1,0,0));
	shared_ptr<Material> material_gray  = make_shared<Lambertian>(Color(0.5,0.5,0.5));

	shared_ptr<Material> material_magenta = make_shared<Lambertian>(Color(0.7,0,0.7));

	shared_ptr<Material> difflight_magenta = make_shared<Diffuse_Light>(Color(6,0,6));
	shared_ptr<Material> difflight_green   = make_shared<Diffuse_Light>(Color(0,3,0));
	shared_ptr<Material> difflight_white   = make_shared<Diffuse_Light>(Color(3,3,3));
	shared_ptr<Material> difflight_blue    = make_shared<Diffuse_Light>(Color(0,0,3));
	shared_ptr<Material> difflight_red     = make_shared<Diffuse_Light>(Color(3,0,0));

	shared_ptr<Material> difflight_magenta_m = make_shared<Diffuse_Light>(Color(7,0,6));
	shared_ptr<Material> difflight_w_low     = make_shared<Diffuse_Light>(Color(1,1,1));
	shared_ptr<Material> difflight_d         = make_shared<Diffuse_Light>(Color(0,0,0));
	shared_ptr<Material> difflight_w         = make_shared<Diffuse_Light>(Color(37,37,37));

////////////////////////////////////////////////////////////////////////////

	world.add(make_shared<Obj>(embree, "F:\\cpp\\ray_t\\Ray-tracer\\src\\models\\utah_teapot.obj", "../../materials/", material_red));

////////////////////////////////////////////////////////////////////////////

	return world;
}

// 7680
// 1337