#pragma once

#include <vector>

#include "Color.h"
#include "Settings.h"
#include "Utility_functions.h"

enum Kernel_Types {
	T_CUSTOM          = 0,
	T_SHARPEN         = 1,
	T_SOBEL_T         = 2,
	T_SOBEL_B         = 3,
	T_SOBEL_L         = 4,
	T_SOBEL_R         = 5,
	T_OUTLINE         = 6,
	T_GAUSSIANBLUR3X3 = 7
};

class Convolution {
	public:
		Convolution(std::vector<Color>& pixels) {
			copy_pixels.resize(pixels.size());
		}
		~Convolution() {}

		void matrix_filtering(int kernel_type, std::vector<Color>& pixels) {
			if(kernel_type == -1) { return; }

			float* kernel = nullptr;

			if(kernel_type == T_CUSTOM)          { kernel = Custom; }
			if(kernel_type == T_SHARPEN)         { kernel = Sharpen; }
			if(kernel_type == T_SOBEL_T)         { kernel = Sobel_t; }
			if(kernel_type == T_SOBEL_B)         { kernel = Sobel_b; }
			if(kernel_type == T_SOBEL_L)         { kernel = Sobel_l; }
			if(kernel_type == T_SOBEL_R)         { kernel = Sobel_r; }
			if(kernel_type == T_OUTLINE)         { kernel = Outline; }
			if(kernel_type == T_GAUSSIANBLUR3X3) { kernel = GaussianBlur3x3; }

			for(int y = 0; y < image_height; y++) {
				for(int x = 0; x < image_width; x++) {
					filter(pixels, kernel, x, y);
				}
			}
		}

	private:
		void filter(std::vector<Color>& pixels , float* kernel, int x, int y) {
			Color temp(0.f, 0.f, 0.f);

			if(y - 1 > 0) {
				if(x - 1 > 0)           { temp += kernel[0] * pixels[(y - 1) * image_width + x - 1]; }
				if(x + 1 < image_width) { temp += kernel[2] * pixels[(y - 1) * image_width + x + 1]; }
										  temp += kernel[1] * pixels[(y - 1) * image_width + x + 0];
			}

									  temp += kernel[4] * pixels[y * image_width + x + 0];
			if(x - 1 > 0)           { temp += kernel[3] * pixels[y * image_width + x - 1]; }
			if(x + 1 < image_width) { temp += kernel[5] * pixels[y * image_width + x + 1]; }
			

			if(y + 1 < image_height) {
				if(x - 1 > 0)           { temp += kernel[6] * pixels[(y + 1) * image_width + x - 1]; }
				if(x + 1 < image_width) { temp += kernel[8] * pixels[(y + 1) * image_width + x + 1]; }
										  temp += kernel[7] * pixels[(y + 1) * image_width + x + 0];
			}

			temp.e[0] = clamp(temp.e[0], 0, 255);
			temp.e[1] = clamp(temp.e[1], 0, 255);
			temp.e[2] = clamp(temp.e[2], 0, 255);

			copy_pixels[y * image_width + x] = temp;
		}

	public:
		std::vector<Color> copy_pixels;

		bool filtered = false;
};