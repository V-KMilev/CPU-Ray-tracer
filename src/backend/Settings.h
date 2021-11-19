#pragma once

#include "Vec.h"
#include "Color.h"

#define RES_DEFAULT 400

#define RES_TEST 800

#define RES_HD 1080
#define RES_FULL_HD 1920
#define RES_2K 2048
#define RES_4K 3840

/* Basic parameters | Default settings */
float aspect_ratio = {1.0 / 1.0};                                    // Image: Aspect ratio: resolution
int image_width = {RES_DEFAULT};                                          // Image: Width
int image_height = {static_cast<int>(image_width / aspect_ratio)};    // Image: Height

int samples_per_pixel = {5};                                          // Rays per pixel
int max_depth = {2};                                                  // Ray bounce limit per pixel

std::vector<Color> pixels(image_width * image_height);                // Image: output pixels
std::vector<int> spp(image_width * image_height);                     // Image: output pixels

/* Camera parameters */
Point lookfrom(1.9, 7.0, -1.9);
Point lookat(-5.0, 6.0, 5.0);
Vec view_up(0.0, 1.0, 0.0);

float dist_to_focus = 4.5;
float aperture = 0.1;

/* Background parameters */
Color background(0.0, 0.0, 0.0);
// (0.5, 0.0, 0.5)
