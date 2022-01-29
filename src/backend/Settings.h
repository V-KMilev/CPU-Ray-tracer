#pragma once

#include "Embree_setup.h"
#include "Camera.h"
#include "Color.h"
#include "Vec.h"

#include <atomic>
#include <thread>

#define RES_DEFAULT 400

#define RES_TEST 800

#define RES_HD 1080
#define RES_FULL_HD 1920
#define RES_2K 2048
#define RES_4K 3840

/* Basic parameters | Default settings */
float aspect_ratio = {16.0 / 9.0};                                     // Image: aspect ratio: resolution

int image_width = {RES_TEST};                                          // Image: width
int image_height = {static_cast<int>(image_width / aspect_ratio)};     // Image: height

int samples_per_pixel = {1};                                           // rays per pixel
int max_depth = {2};                                                   // ray bounce limit per pixel

const int bucket_size = { 64 };                                        // bucket size x by y

int total_buckets = {0};                                               // total amount of buckets

const int MAX_NUMBER_OF_THREADS = std::thread::hardware_concurrency(); // max number of threads

/* Image pixels*/
std::vector<Color> pixels(image_width * image_height);                 // Image: output pixels
std::vector<int> samples_in_pixels(image_width * image_height);        // Image: samples in pixel

std::vector<Color> empty_pixels(image_width * image_height);           // Image: output pixels = (0.0f ,0.0f ,0.0f)
std::vector<int> empty_samples_in_pixels(image_width * image_height);  // Image: samples in pixel = 0 ... 0

/* Background parameters */
Color background(0.0, 0.0, 0.0);

/* Camera parameters */
Point lookfrom(0.0, 1.0, -3.0);
Point lookat(0.0, 0.0, 0.0);
Vec view_up(0.0, 1.0, 0.0);

float dist_to_focus = 4.5;
float aperture = 0.1;

/* Render parameters */
std::atomic<int> samples_in_counter = {0};

/* Event flags */
bool change_position = false;
bool change_view = false;
bool change_bg = false;
bool change_default = false;
bool change_stop = true;
bool change_clear = false;
bool change_multithreading = false;

/* Default settings */
float precision = 0.1;

int default_samples_per_pixel = {1};
int default_max_depth = {2};

Point default_lookfrom(0.0, 1.0, 0.0);
Point default_lookat(1.0, 1.0, 0.0);

Color default_background(0.0, 0.0, 0.0);

MyEmbree embree;