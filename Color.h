#pragma once

#include <fstream>

#include "Vec.h"

void write_color(std::ostream &out, Color pixel_color, int samples_per_pixel) {
    float r = pixel_color.getX();
    float g = pixel_color.getY();
    float b = pixel_color.getZ();

    // Divide the color by the number of samples.
    float scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}