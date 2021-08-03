#pragma once

#include <fstream>

#include "Vec.h"

void write_color(std::ostream &out, Color pixel_color, int samples_per_pixel) {
    float red = pixel_color.getX();
    float green = pixel_color.getY();
    float blue = pixel_color.getZ();

    // Divide the color by the number of samples.
    float scale = 1.0 / samples_per_pixel;
    red   *= scale;
    green *= scale;
    blue  *= scale;

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(red, 0.0, 0.999))   << ' '
        << static_cast<int>(256 * clamp(green, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(blue, 0.0, 0.999))  << '\n';
}