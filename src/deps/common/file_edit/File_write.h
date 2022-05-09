#pragma once

#include <fstream>

#include "Utility_functions.h"
#include "Color.h"

/*
 * Function - file_write
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

void file_write(std::ofstream &out, std::vector<Color> pixels, const int image_width, const int image_height) {

	out << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int idx = 0; idx < pixels.size(); idx++) {

		out << static_cast<int>(256 * clamp(pixels[idx].getRed(), 0.0f, 1.0f - epsilon)) << ' '
			<< static_cast<int>(256 * clamp(pixels[idx].getGreen(), 0.0f, 1.0f - epsilon)) << ' '
			<< static_cast<int>(256 * clamp(pixels[idx].getBlue(), 0.0f, 1.0f - epsilon)) << '\n';
	}

	out.close();
}
