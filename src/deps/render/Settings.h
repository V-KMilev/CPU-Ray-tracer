#pragma once

#include "Embree_setup.h"
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
const int MAX_NUMBER_OF_THREADS = std::thread::hardware_concurrency(); // max number of threads

const int bucket_size = {64};                                          // bucket size x by y

const float aspect_ratio = {16.0 / 9.0};                               // Image: aspect ratio: resolution

int image_width = {RES_HD};                                            // Image: width
int image_height = {static_cast<int>(image_width / aspect_ratio)};     // Image: height

int samples_per_pixel = {1};                                           // rays per pixel
int max_depth = {2};                                                   // ray bounce limit per pixel

int total_buckets = {0};                                               // total amount of buckets

MyEmbree embree;

//////////////////////////////////////////////////////////////

/* Image pixels*/
std::vector<Color> pixels(image_width * image_height);                 // Image: output pixels
std::vector<int> samples_in_pixels(image_width * image_height);        // Image: samples in pixel

std::vector<Color> empty_pixels(image_width * image_height);           // Image: output pixels = (0.0f ,0.0f ,0.0f)
std::vector<int> empty_samples_in_pixels(image_width * image_height);  // Image: samples in pixel = 0 ... 0

//////////////////////////////////////////////////////////////

/* Background parameters */
Color background(0.0f, 0.0f, 0.0f);

//////////////////////////////////////////////////////////////

/* Camera parameters */
Point lookfrom(-2.0f, 1.0f, 2.0f);
Point lookat(0.0f, 1.0f, 0.0f);
Vec view_up(0.0f, 1.0f, 0.0f);

float dist_to_focus = 3.5f;
float aperture = 0.1f;

//////////////////////////////////////////////////////////////

/* Move parameters */
float precision = {0.1f};

//////////////////////////////////////////////////////////////

/* Render parameters */
std::atomic<int> samples_in_counter = {0};

//////////////////////////////////////////////////////////////

/* Event flags */
bool change_multithreading = false;
bool change_position       = false;
bool change_aperture       = false;
bool change_default        = false;
bool change_static         = false;
bool change_clear          = false;
bool change_view           = false;
bool change_bg             = false;
bool change_run            = true;

//////////////////////////////////////////////////////////////

/* Default settings */
int default_samples_per_pixel = {1};
int default_max_depth = {2};

Point default_lookfrom(0.0f, 1.0f, 0.0f);
Point default_lookat(1.0f, 1.0f, 0.0f);

Color default_background(0.0f, 0.0f, 0.0f);
