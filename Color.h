#pragma once

#include <fstream>
#include <vector>

#include "Vec.h"

Color color_gama(Color pixel_color, int samples_per_pixel) {
	
	float red   = pixel_color.getX();
	float green = pixel_color.getY();
	float blue  = pixel_color.getZ();

	// Divide the color by the number of samples and gamma-correct for gamma=2.0. - GAMMA 2
	float scale = 1.0 / samples_per_pixel;
	red   = sqrt(scale * red);
	green = sqrt(scale * green);
	blue  = sqrt(scale * blue);

	return Color(red, green, blue);

}
