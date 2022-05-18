#pragma once

#include <iostream>

#include "Utility_functions.h"
#include "Color.h"
#include "Vec.h"

#include "RT_stb_image.h"

enum Texture_ID : unsigned int {
	t_solid_color     = 1,
	t_checker_texture = 2,
	t_image_texture   = 3
};


class Texture {    // Interface
	public:

		/*
		 * Function - value
		 *
		 * Parameters:
		 * [p] float u,
		 * [p] float v,
		 * [p] const Point &point
		 * 
		 * Return type:
		 * Color
		 * 
		 * Use:
		 * Base funtion (virtual). 
		 * Value is used to get the pixel Color value.
		 */

		virtual Color value(float u, float v, const Point &point) const = 0;

	public:
		const char* my_name;
		Texture_ID id;
};

class Solid_Color : public Texture {
	public:
		Solid_Color() {
			my_name = "Solid-Color";
			id = Texture_ID::t_solid_color;
		}

		// Base constructor for color_value
		Solid_Color(Color color) : color_value(color) {
			my_name = "Solid-Color";
			id = Texture_ID::t_solid_color;
		}

		// Constructor to set color_value
		Solid_Color(float red, float green, float blue)
		: Solid_Color(Color(red,green,blue)) {
			my_name = "Solid-Color";
			id = Texture_ID::t_solid_color;
		}

		/*
		 * Function - value
		 *
		 * Use:
		 * Override of base funtion. 
		 * return the Color value of color_value.
		 */

		virtual Color value(float u, float v, const Point &point) const override {
			return color_value;
		}

	public:
		Color color_value;    // Texture color
};

class Checker_Texture : public Texture {
	public:
		Checker_Texture() {
				my_name = "Checker-Texture";
				id = Texture_ID::t_checker_texture;
		}

		// Constructor to init the even and odd Textures
		Checker_Texture(shared_ptr<Solid_Color> even, shared_ptr<Solid_Color> odd)
			: even(even), odd(odd) {
				my_name = "Checker-Texture";
				id = Texture_ID::t_checker_texture;
			}

		// Constructor to set the even and odd Textures
		Checker_Texture(Color color_0, Color color_1)
			: even(make_shared<Solid_Color>(color_0)), odd(make_shared<Solid_Color>(color_1)) {
				my_name = "Checker-Texture";
				id = Texture_ID::t_checker_texture;
			}

		virtual Color value(float u, float v, const Point &point) const override {

			float sines = 
				sin(10 * point.getX()) *
				sin(10 * point.getY()) *
				sin(10 * point.getZ());

			if(sines < 0) { return odd->value(u, v, point); }
			else { return even->value(u, v, point); }
		}

	public:
		shared_ptr<Solid_Color> odd;     // odd Texture
		shared_ptr<Solid_Color> even;    // even Texture
};

class Image_Texture : public Texture {
	public:
		const static int bytes_per_pixel = 3;

		Image_Texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {
			my_name = "Image_Texture";
			id = Texture_ID::t_image_texture;
		}

		Image_Texture(const char* file_name = "") {
			my_name = "Image_Texture";
			id = Texture_ID::t_image_texture;

			strcpy(my_file_name, file_name);

			int components_per_pixel = bytes_per_pixel;

			data = stbi_load(file_name, &width, &height, &components_per_pixel, components_per_pixel);

			if(!data) {
				std::cerr << "WARNING: Could not load texture image file " << file_name << "\n STB Reason: " << stbi_failure_reason() << "\n";
				width  = 0;
				height = 0;
			}

			bytes_per_scanline = bytes_per_pixel * width;
		}

		~Image_Texture() {
			delete data;
		}

		virtual Color value(float u, float v, const Point &point)  const override {
			// If we have no texture data, then return solid cyan as a debugging aid.
			if(data == nullptr) {
				return Color(1,0,0);
			}

			// Clamp input texture coordinates to [0,1] x [1,0]
			u = clamp(u, 0.0, 1.0);
			v = 1.0 - clamp(v, 0.0, 1.0); // Flip V to image coordinates

			int i = static_cast<int>(u * width);
			int j = static_cast<int>(v * height);

			// Clamp integer mapping, since actual coordinates should be less than 1.0
			if(i >= width) {
				i = width - 1;
			}
			if(j >= height) {
				j = height - 1;
			}

			const float color_scale = 1.0 / 255.0;
			unsigned char *pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

			return Color(
				color_scale * pixel[0],
				color_scale * pixel[1],
				color_scale * pixel[2]
				);
		}

	public:
		char my_file_name[255];

		unsigned char *data;
		int width, height;
		int bytes_per_scanline;
};
