#pragma once

#include <fstream>

#include "Utility_functions.h"
#include "Settings.h"
#include "Color.h"
#include "World.h"

/*
 * Function - image_file_write
 *
 * Parameters:
 * [p] std::ofstream &out,
 * [p] std::vector<Color> pixels,
 * [p] const int image_width,
 * [p] const int image_height
 * 
 * Return type:
 * void
 * 
 * Use:
 * By giving a out stream you set a file to write on. 
 * The vector that you give is the PPM format pixels that 
 * are going to be written. 
 * Image_width is the width of the image. 
 * Image_height is the height of the image.
 */

void image_file_write(std::ofstream &out, std::vector<Color> pixels, const int image_width, const int image_height) {

	out << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int idx = 0; idx < pixels.size(); idx++) {

		out << static_cast<int>(256 * clamp(pixels[idx].getRed(),   0.0f, 1.0f - epsilon)) << ' '
			<< static_cast<int>(256 * clamp(pixels[idx].getGreen(), 0.0f, 1.0f - epsilon)) << ' '
			<< static_cast<int>(256 * clamp(pixels[idx].getBlue(),  0.0f, 1.0f - epsilon)) << '\n';
	}

	out.close();
}

template<class T>
void object_values(std::ofstream &out, T* object) {

	out << object->id << " ";

	if(object->material_ptr->id == t_lambertian) {
		Lambertian* material = static_cast<Lambertian*>(object->material_ptr.get());

		out << material->id << " ";

		if(material->albedo->id == t_solid_color) {
			Solid_Color* texture = static_cast<Solid_Color*>(material->albedo.get());

			out << texture->id << " ";
			out << texture->color_value[0] << " " << texture->color_value[1] << " " << texture->color_value[2] << " ";
		}
		if(material->albedo->id == t_checker_texture) {
			Checker_Texture* texture = static_cast<Checker_Texture*>(material->albedo.get());

			out << texture->id << " ";
			out << texture->odd->color_value[0] << " " <<  texture->odd->color_value[1] << " " <<  texture->odd->color_value[2] << " "
				<< texture->even->color_value[0] << " " <<  texture->even->color_value[1] << " " <<  texture->even->color_value[2] << " ";
		}
		if(material->albedo->id == t_image_texture) {
			Image_Texture* texture = static_cast<Image_Texture*>(material->albedo.get());

			out << texture->id << " ";
			out << texture->my_file_name << " ";
		}
	}

	if(object->material_ptr->id == t_diffuse_light) {
		Diffuse_Light* material = static_cast<Diffuse_Light*>(object->material_ptr.get());

		out << material->id << " ";

		if(material->emit->id == t_solid_color) {
			Solid_Color* texture = static_cast<Solid_Color*>(material->emit.get());

			out << texture->id << " ";
			out << texture->color_value[0] << " " << texture->color_value[1] << " " << texture->color_value[2] << " ";
		}
		if(material->emit->id == t_checker_texture) {
			Checker_Texture* texture = static_cast<Checker_Texture*>(material->emit.get());

			out << texture->id << " ";
			out << texture->odd->color_value[0] << " " <<  texture->odd->color_value[1] << " " <<  texture->odd->color_value[2] << " "
				<< texture->even->color_value[0] << " " <<  texture->even->color_value[1] << " " <<  texture->even->color_value[2] << " ";
		}
		if(material->emit->id == t_image_texture) {
			Image_Texture* texture = static_cast<Image_Texture*>(material->emit.get());

			out << texture->id << " ";
			out << texture->my_file_name << " ";
		}
	}
}

/*
 * Function - scene_file_write
 *
 * Parameters:
 * [p] std::ofstream &out
 * 
 * Return type:
 * void
 * 
 * Use:
 * By giving a out stream you set a file to write on. 
 * tObject format: 
 * Object_ID Material_ID Texture_ID [Texture_value] [Position] [Unique Parameters]\n
 */

void scene_file_write(std::ofstream &out) {

	std::vector<shared_ptr<Hittable>> &my_objects = world.objects;

	for(shared_ptr<Hittable>& object : my_objects) {
		if(object->id == t_xy_rect) {
			xy_rect* m_object = static_cast<xy_rect*>(object.get());

			object_values<xy_rect>(out, m_object);

			out << m_object->x_start << " " << m_object->x_end << " "
				<< m_object->y_start << " " << m_object->y_end << " "
				<< m_object->z << "\n";
		}
		else if(object->id == t_xz_rect) {
			xz_rect* m_object = static_cast<xz_rect*>(object.get());

			object_values<xz_rect>(out, m_object);

			out << m_object->x_start << " " << m_object->x_end << " "
				<< m_object->z_start << " " << m_object->z_end << " "
				<< m_object->y << "\n";
		}
		else if(object->id == t_yz_rect) {
			yz_rect* m_object = static_cast<yz_rect*>(object.get());

			object_values<yz_rect>(out, m_object);

			out << m_object->y_start << " " << m_object->y_end << " "
				<< m_object->z_start << " " << m_object->z_end << " "
				<< m_object->x << "\n";
		}
		else if(object->id == t_box) {
			Box* m_object = static_cast<Box*>(object.get());

			object_values<Box>(out, m_object);

			out << m_object->start.getX() << " " << m_object->start.getY() << " " << m_object->start.getZ() << " "
				<< m_object->end.getX()   << " " << m_object->end.getY()   << " " << m_object->end.getZ()   << "\n";
		}
		else if(object->id == t_obj) {
			Obj* m_object = static_cast<Obj*>(object.get());

			object_values<Obj>(out, m_object);

			out << m_object->filePath <<"\n";
		}
		else if(object->id == t_sphere_moving) {
			Sphere_moving* m_object = static_cast<Sphere_moving*>(object.get());

			object_values<Sphere_moving>(out, m_object);

			out << m_object->center_0.getX() << " " << m_object->center_0.getY() << " " << m_object->center_0.getZ() << " "
				<< m_object->center_1.getX() << " " << m_object->center_1.getY() << " " << m_object->center_1.getZ() << " "
				<< m_object->radius << "\n";
		}
		else if(object->id == t_sphere) {
			Sphere* m_object = static_cast<Sphere*>(object.get());

			object_values<Sphere>(out, m_object);

			out << m_object->center.getX() << " " << m_object->center.getY() << " " << m_object->center.getZ() << " "
				<< m_object->radius << "\n";
		}
	}

	out.close();
}
