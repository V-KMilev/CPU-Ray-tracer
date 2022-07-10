#pragma once

#include <fstream>

#include "Utility_functions.h"
#include "Settings.h"
#include "Color.h"
#include "World.h"

/*
 * Function - fileToString
 *
 * Parameters:
 * [p] const std::string& file_name
 * 
 * Return type:
 * const std:string
 * 
 * Use:
 * By giving a file name (file path) to the 
 * funtion you, get in return the content 
 * of the file in string form.
 */

const std::string fileToString(const std::string &file_name) {

	FILE *file = fopen(&file_name[0], "r");

	std::string buffer;    // you can also use buffer(file_size, ' ')

	if(file) {
		fseek(file, 0, SEEK_END);

		size_t file_size = ftell(file);

		fseek(file, 0, SEEK_SET);

		buffer.resize(file_size);

		fread(&buffer[0], 1, file_size, file);
		fclose(file);
	}

	return buffer;
}

/*
 * Function - file_lines_count
 *
 * Parameters:
 * [p] const std::string &string_file
 * 
 * Return type:
 * const unsigned int
 * 
 * Use:
 * Returns total count of the line from 
 * the string you've set.
 */

const unsigned int file_lines_count(const std::string &string_file) {
	unsigned int lines = 0;

	for(int i = 0; i < string_file.size(); i++) {
		if(string_file[i] == '\n') {
			lines++;
		}
	}
	return lines;
}


/*
 * Function - load_scene
 *
 * Parameters:
 * [p] std::ofstream &out
 * 
 * Return type:
 * void
 * 
 * Use:
 * Adds all the objects written in the given file. 
 * Object format: 
 * Object_ID Material_ID Texture_ID [Texture_value] [Position] [Unique Parameters]\n
 */

void load_scene(const std::string &file_scene) {

	FILE *file = fopen(&file_scene[0], "r");

	if (file) {
		const std::string string_scene = fileToString(file_scene);

		std::cerr << "[SCENE][LOADING] load_scene - File: " <<  file_scene << " is loading...\n";

		unsigned int lines = file_lines_count(string_scene);

		unsigned int object_id   = 0;
		unsigned int material_id = 0;
		unsigned int texture_id  = 0;

		float s_x = 0.f;
		float e_x = 0.f;
		float s_y = 0.f;
		float e_y = 0.f;
		float s_z = 0.f;
		float e_z = 0.f;

		float radius = 0.f;

		float r_0 = 0.f;
		float g_0 = 0.f;
		float b_0 = 0.f;
		float r_1 = 0.f;
		float g_1 = 0.f;
		float b_1 = 0.f;

		char object_file_name[255] = "";
		char image_name[255]       = "";

		for(int i = 0; i < lines; i++) {

			fscanf(file, "%d %d %d", &object_id, &material_id, &texture_id);

			if(material_id == t_lambertian) {
				if(texture_id == t_solid_color) {
					fscanf(file, "%f %f %f", &r_0, &g_0, &b_0);
				}
				else if(texture_id == t_checker_texture) {
					fscanf(file, "%f %f %f %f %f %f", &r_0, &g_0, &b_0, &r_1, &g_1, &b_1);
				}
				else if(texture_id == t_image_texture) {
					fscanf(file, "%s", &image_name);
				}
			}
			else if(material_id == t_diffuse_light) {
				if(texture_id == t_solid_color) {
					fscanf(file, "%f %f %f", &r_0, &g_0, &b_0);
				}
				else if(texture_id == t_checker_texture) {
					fscanf(file, "%f %f %f %f %f %f", &r_0, &g_0, &b_0, &r_1, &g_1, &b_1);
				}
				else if(texture_id == t_image_texture) {
					fscanf(file, "%s", &image_name);
				}
			}

			if(object_id == t_xy_rect) {
				fscanf(file, "%f %f %f %f %f", &s_x, &e_x, &s_y, &e_y, &s_z);

				switch(material_id) {
					case t_lambertian:
						switch(texture_id) {
							case t_solid_color:
								world.add(make_shared<xy_rect>(s_x, e_x, s_y, e_y, s_z,
									make_shared<Lambertian>(make_shared<Solid_Color>(r_0, g_0, b_0))));
							break;
							case t_checker_texture:
								world.add(make_shared<xy_rect>(s_x, e_x, s_y, e_y, s_z,
									make_shared<Lambertian>(make_shared<Checker_Texture>(Color(r_0, g_0, b_0), Color(r_1, g_1, b_1)))));
							break;
							case t_image_texture:
								world.add(make_shared<xy_rect>(s_x, e_x, s_y, e_y, s_z,
									make_shared<Lambertian>(make_shared<Image_Texture>(image_name))));
							break;
						}
					break;
					case t_diffuse_light:
						switch(texture_id) {
							case t_solid_color:
								world.add(make_shared<xy_rect>(s_x, e_x, s_y, e_y, s_z,
									make_shared<Diffuse_Light>(make_shared<Solid_Color>(r_0, g_0, b_0))));
							break;
							case t_checker_texture:
								world.add(make_shared<xy_rect>(s_x, e_x, s_y, e_y, s_z,
									make_shared<Diffuse_Light>(make_shared<Checker_Texture>(Color(r_0, g_0, b_0), Color(r_1, g_1, b_1)))));
							break;
							case t_image_texture:
								world.add(make_shared<xy_rect>(s_x, e_x, s_y, e_y, s_z,
									make_shared<Diffuse_Light>(make_shared<Image_Texture>(image_name))));
							break;
						}
					break;
				}
			}
			else if(object_id == t_xz_rect) {
				fscanf(file, "%f %f %f %f %f", &s_x, &e_x, &s_z, &e_z, &s_y);

				switch(material_id) {
					case t_lambertian:
						switch(texture_id) {
							case t_solid_color:
								world.add(make_shared<xz_rect>(s_x, e_x, s_z, e_z, s_y,
									make_shared<Lambertian>(make_shared<Solid_Color>(r_0, g_0, b_0))));
							break;
							case t_checker_texture:
								world.add(make_shared<xz_rect>(s_x, e_x, s_z, e_z, s_y,
									make_shared<Lambertian>(make_shared<Checker_Texture>(Color(r_0, g_0, b_0), Color(r_1, g_1, b_1)))));
							break;
							case t_image_texture:
								world.add(make_shared<xz_rect>(s_x, e_x, s_z, e_z, s_y,
									make_shared<Lambertian>(make_shared<Image_Texture>(image_name))));
							break;
						}
					break;
					case t_diffuse_light:
						switch(texture_id) {
							case t_solid_color:
								world.add(make_shared<xz_rect>(s_x, e_x, s_z, e_z, s_y,
									make_shared<Diffuse_Light>(make_shared<Solid_Color>(r_0, g_0, b_0))));
							break;
							case t_checker_texture:
								world.add(make_shared<xz_rect>(s_x, e_x, s_z, e_z, s_y,
									make_shared<Diffuse_Light>(make_shared<Checker_Texture>(Color(r_0, g_0, b_0), Color(r_1, g_1, b_1)))));
							break;
							case t_image_texture:
								world.add(make_shared<xz_rect>(s_x, e_x, s_z, e_z, s_y,
									make_shared<Diffuse_Light>(make_shared<Image_Texture>(image_name))));
							break;
						}
					break;
				}
			}
			else if(object_id == t_yz_rect) {
				fscanf(file, "%f %f %f %f %f", &s_y, &e_y, &s_z, &e_z, &s_x);

				switch(material_id) {
					case t_lambertian:
						switch(texture_id) {
							case t_solid_color:
								world.add(make_shared<yz_rect>(s_y, e_y, s_z, e_z, s_x,
									make_shared<Lambertian>(make_shared<Solid_Color>(r_0, g_0, b_0))));
							break;
							case t_checker_texture:
								world.add(make_shared<yz_rect>(s_y, e_y, s_z, e_z, s_x,
									make_shared<Lambertian>(make_shared<Checker_Texture>(Color(r_0, g_0, b_0), Color(r_1, g_1, b_1)))));
							break;
							case t_image_texture:
								world.add(make_shared<yz_rect>(s_y, e_y, s_z, e_z, s_x,
									make_shared<Lambertian>(make_shared<Image_Texture>(image_name))));
							break;
						}
					break;
					case t_diffuse_light:
						switch(texture_id) {
							case t_solid_color:
								world.add(make_shared<yz_rect>(s_y, e_y, s_z, e_z, s_x,
									make_shared<Diffuse_Light>(make_shared<Solid_Color>(r_0, g_0, b_0))));
							break;
							case t_checker_texture:
								world.add(make_shared<yz_rect>(s_y, e_y, s_z, e_z, s_x,
									make_shared<Diffuse_Light>(make_shared<Checker_Texture>(Color(r_0, g_0, b_0), Color(r_1, g_1, b_1)))));
							break;
							case t_image_texture:
								world.add(make_shared<yz_rect>(s_y, e_y, s_z, e_z, s_x,
									make_shared<Diffuse_Light>(make_shared<Image_Texture>(image_name))));
							break;
						}
					break;
				}
			}
			else if(object_id == t_box) {
				fscanf(file, "%f %f %f %f %f %f", &s_x, &s_y, &s_z, &e_x, &e_y, &e_z);

				const Point start_point(s_x, s_y, s_z);
				const Point end_point(e_x, e_y, e_z);

				switch(material_id) {
					case t_lambertian:
						switch(texture_id) {
							case t_solid_color:
								world.add(make_shared<Box>(start_point, end_point,
									make_shared<Lambertian>(make_shared<Solid_Color>(r_0, g_0, b_0))));
							break;
							case t_checker_texture:
								world.add(make_shared<Box>(start_point, end_point,
									make_shared<Lambertian>(make_shared<Checker_Texture>(Color(r_0, g_0, b_0), Color(r_1, g_1, b_1)))));
							break;
							case t_image_texture:
								world.add(make_shared<Box>(start_point, end_point,
									make_shared<Lambertian>(make_shared<Image_Texture>(image_name))));
							break;
						}
					break;
					case t_diffuse_light:
						switch(texture_id) {
							case t_solid_color:
								world.add(make_shared<Box>(start_point, end_point,
									make_shared<Diffuse_Light>(make_shared<Solid_Color>(r_0, g_0, b_0))));
							break;
							case t_checker_texture:
								world.add(make_shared<Box>(start_point, end_point,
									make_shared<Diffuse_Light>(make_shared<Checker_Texture>(Color(r_0, g_0, b_0), Color(r_1, g_1, b_1)))));
							break;
							case t_image_texture:
								world.add(make_shared<Box>(start_point, end_point,
									make_shared<Diffuse_Light>(make_shared<Image_Texture>(image_name))));
							break;
						}
					break;
				}
			}
			else if(object_id == t_obj) {
				fscanf(file, "%s", &object_file_name);

				switch(material_id) {
					case t_lambertian:
						switch(texture_id) {
							case t_solid_color:
								world.add(make_shared<Obj>(embree, object_file_name, object_file_name,
									make_shared<Lambertian>(make_shared<Solid_Color>(r_0, g_0, b_0))));
							break;
							case t_checker_texture:
								world.add(make_shared<Obj>(embree, object_file_name, object_file_name,
									make_shared<Lambertian>(make_shared<Checker_Texture>(Color(r_0, g_0, b_0), Color(r_1, g_1, b_1)))));
							break;
							case t_image_texture:
								world.add(make_shared<Obj>(embree, object_file_name, object_file_name,
									make_shared<Lambertian>(make_shared<Image_Texture>(image_name))));
							break;
						}
					break;
					case t_diffuse_light:
						switch(texture_id) {
							case t_solid_color:
								world.add(make_shared<Obj>(embree, object_file_name, object_file_name,
									make_shared<Diffuse_Light>(make_shared<Solid_Color>(r_0, g_0, b_0))));
							break;
							case t_checker_texture:
								world.add(make_shared<Obj>(embree, object_file_name, object_file_name,
									make_shared<Diffuse_Light>(make_shared<Checker_Texture>(Color(r_0, g_0, b_0), Color(r_1, g_1, b_1)))));
							break;
							case t_image_texture:
								world.add(make_shared<Obj>(embree, object_file_name, object_file_name,
									make_shared<Diffuse_Light>(make_shared<Image_Texture>(image_name))));
							break;
						}
					break;
				}
			}
			else if(object_id == t_sphere_moving) {
				fscanf(file, "%f %f %f %f %f %f %f", &s_x, &s_y, &s_z, &e_x, &e_y, &e_z, &radius);

				switch(material_id) {
					case t_lambertian:
						switch(texture_id) {
							case t_solid_color:
								world.add(make_shared<Sphere_moving>(Point(s_x, s_y, s_z), Point(e_x, e_y, e_z), 0, 0, radius,
									make_shared<Lambertian>(make_shared<Solid_Color>(r_0, g_0, b_0))));
							break;
							case t_checker_texture:
								world.add(make_shared<Sphere_moving>(Point(s_x, s_y, s_z), Point(e_x, e_y, e_z), 0, 0, radius,
									make_shared<Lambertian>(make_shared<Checker_Texture>(Color(r_0, g_0, b_0), Color(r_1, g_1, b_1)))));
							break;
							case t_image_texture:
								world.add(make_shared<Sphere_moving>(Point(s_x, s_y, s_z), Point(e_x, e_y, e_z), 0, 0, radius,
									make_shared<Lambertian>(make_shared<Image_Texture>(image_name))));
							break;
						}
					break;
					case t_diffuse_light:
						switch(texture_id) {
							case t_solid_color:
								world.add(make_shared<Sphere_moving>(Point(s_x, s_y, s_z), Point(e_x, e_y, e_z), 0, 0, radius,
									make_shared<Diffuse_Light>(make_shared<Solid_Color>(r_0, g_0, b_0))));
							break;
							case t_checker_texture:
								world.add(make_shared<Sphere_moving>(Point(s_x, s_y, s_z), Point(e_x, e_y, e_z), 0, 0, radius,
									make_shared<Diffuse_Light>(make_shared<Checker_Texture>(Color(r_0, g_0, b_0), Color(r_1, g_1, b_1)))));
							break;
							case t_image_texture:
								world.add(make_shared<Sphere_moving>(Point(s_x, s_y, s_z), Point(e_x, e_y, e_z), 0, 0, radius,
									make_shared<Diffuse_Light>(make_shared<Image_Texture>(image_name))));
							break;
						}
					break;
				}
			}
			else if(object_id == t_sphere) {
				fscanf(file, "%f %f %f %f", &s_x, &s_y, &s_z, &radius);

				switch(material_id) {
					case t_lambertian:
						switch(texture_id) {
							case t_solid_color:
								world.add(make_shared<Sphere>(Point(s_x, s_y, s_z), radius,
									make_shared<Lambertian>(make_shared<Solid_Color>(r_0, g_0, b_0))));
							break;
							case t_checker_texture:
								world.add(make_shared<Sphere>(Point(s_x, s_y, s_z), radius,
									make_shared<Lambertian>(make_shared<Checker_Texture>(Color(r_0, g_0, b_0), Color(r_1, g_1, b_1)))));
							break;
							case t_image_texture:
								world.add(make_shared<Sphere>(Point(s_x, s_y, s_z), radius,
									make_shared<Lambertian>(make_shared<Image_Texture>(image_name))));
							break;
						}
					break;
					case t_diffuse_light:
						switch(texture_id) {
							case t_solid_color:
								world.add(make_shared<Sphere>(Point(s_x, s_y, s_z), radius,
									make_shared<Diffuse_Light>(make_shared<Solid_Color>(r_0, g_0, b_0))));
							break;
							case t_checker_texture:
								world.add(make_shared<Sphere>(Point(s_x, s_y, s_z), radius,
									make_shared<Diffuse_Light>(make_shared<Checker_Texture>(Color(r_0, g_0, b_0), Color(r_1, g_1, b_1)))));
							break;
							case t_image_texture:
								world.add(make_shared<Sphere>(Point(s_x, s_y, s_z), radius,
									make_shared<Diffuse_Light>(make_shared<Image_Texture>(image_name))));
							break;
						}
					break;
				}
			}
		}
	}
	fclose(file);
	std::cerr << "[SCENE][LOADING] load_scene - File: "<<file_scene << " is loaded!\n";
}
