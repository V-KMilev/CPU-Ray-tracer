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
	return world;
}

// 7680
// 1337