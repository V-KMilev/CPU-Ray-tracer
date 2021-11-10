#pragma once

#include <iostream>

#include "Utility_functions.h"
#include "Vec.h"

#include "r_stb_image.h"
#include "Perlin.h"

class Texture {    // Interface
	public:
		virtual Color value(float u, float v, const Point &point) const = 0;
};

class Solid_Color : public Texture {
	public:
		Solid_Color() {}
		
		// Base constructor for color_value
		Solid_Color(Color color) : color_value(color) {}
		
		// Constructor to set color_value
		Solid_Color(float red, float green, float blue)
			: Solid_Color(Color(red,green,blue)) {}

		virtual Color value(float u, float v, const Point &point) const override {
			return color_value;
		}

	private:
		Color color_value;    // Texture color
};

class Checker_Texture : public Texture {
	public:
		Checker_Texture() {}

		// Constructor to init the even and odd Textures
		Checker_Texture(shared_ptr<Texture> even, shared_ptr<Texture> odd)
			: even(even), odd(odd) {}
		
		// Constructor to set the even and odd Textures
		Checker_Texture(Color color_0, Color color_1)
			: even(make_shared<Solid_Color>(color_0)), odd(make_shared<Solid_Color>(color_1)) {}

		virtual Color value(float u, float v, const Point &point) const override {
			
			float sines = 
				sin(10 * point.getX()) *
				sin(10 * point.getY()) *
				sin(10 * point.getZ());
			
			if (sines < 0) { return odd->value(u, v, point); }
			else { return even->value(u, v, point); }
		}

	private:
		shared_ptr<Texture> odd;     // odd Texture
		shared_ptr<Texture> even;    // even Texture
};

class Noise_Texture : public Texture {
	public:
		Noise_Texture() {}

		virtual Color value(float u, float v, const Point &point) const override {
			return Color(1,1,1) * noise.noise(point);
		}

	public:
		Perlin noise;
};

class Image_Texture : public Texture {
	public:
		const static int bytes_per_pixel = 3;

		Image_Texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

		Image_Texture(const char* filename) {
			int components_per_pixel = bytes_per_pixel;

			data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

			if (!data) {
				fprintf(stderr, "WARNING: Could not load texture image file \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
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
			if (data == nullptr) {
				return Color(1,0,0);
			}

			// Clamp input texture coordinates to [0,1] x [1,0]
			u = clamp(u, 0.0, 1.0);
			v = 1.0 - clamp(v, 0.0, 1.0); // Flip V to image coordinates

			int i = static_cast<int>(u * width);
			int j = static_cast<int>(v * height);

			// Clamp integer mapping, since actual coordinates should be less than 1.0
			if (i >= width) {
				i = width - 1;
			}
			if (j >= height) {
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

	private:
		unsigned char *data;
		int width, height;
		int bytes_per_scanline;
};
