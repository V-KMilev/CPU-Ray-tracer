#pragma once

#include "Vec.h"
#include "Color.h"
#include "Camera.h"

#define RES_DEFAULT 400

#define RES_TEST 800

#define RES_HD 1080
#define RES_FULL_HD 1920
#define RES_2K 2048
#define RES_4K 3840

/* Basic parameters | Default settings */
float aspect_ratio = {16.0 / 9.0};                                    // Image: Aspect ratio: resolution

int image_width = {RES_FULL_HD};                                      // Image: Width
int image_height = {static_cast<int>(image_width / aspect_ratio)};    // Image: Height

int samples_per_pixel = {20};                                         // Rays per pixel
int max_depth = {2};                                                  // Ray bounce limit per pixel

/* Image pixels*/
std::vector<Color> pixels(image_width * image_height);                // Image: output pixels

std::vector<Color> empty_pixels(image_width * image_height);          // Image: (0.0 ,0.0 ,0.0) pixels

std::vector<int> spp_pixels(image_width * image_height);              // Image: samples in pixel

/* Camera parameters */
Point lookfrom(1.9, 7.0, -1.9);
Point lookat(-5.0, 6.0, 5.0);
Vec view_up(0.0, 1.0, 0.0);

float dist_to_focus = 4.5;
float aperture = 0.1;

/* Background parameters */
Color background(0.0, 0.0, 0.0);

/* Change event bools */
bool change_position = false;
bool change_view = false;
bool change_bg = false;
bool change_default = false;
bool change_stop = true;

/* Default settings */
int default_samples_per_pixel = {20};
int default_max_depth = {2};

Point default_lookfrom(1.9, 7.0, -1.9);
Point default_lookat(-5.0, 6.0, 5.0);

Color default_background(0.0, 0.0, 0.0);