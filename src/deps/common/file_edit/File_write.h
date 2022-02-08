#pragma once

#include <fstream>

#include "Color.h"

void file_write(std::ofstream &out, std::vector<Color> pixels, const int image_width, const int image_height) {

	out << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int idx = 0; idx < pixels.size(); idx++) {

		out << static_cast<int>(256 * clamp(pixels[idx].getRed(), 0.0f, 1.0f - epsilon)) << ' '
			<< static_cast<int>(256 * clamp(pixels[idx].getGreen(), 0.0f, 1.0f - epsilon)) << ' '
			<< static_cast<int>(256 * clamp(pixels[idx].getBlue(), 0.0f, 1.0f - epsilon)) << '\n';
	}

	out.close();
}