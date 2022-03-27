#pragma once

#include "Embree_setup.h"
#include "Color.h"
#include "Vec.h"

#include <atomic>
#include <thread>

#define RES_FULL_HD 1920

/* Basic parameters | Default settings */
//////////////////////////////////////////////////////////////

/* Threadpool parameters */
const int MAX_NUMBER_OF_THREADS = std::thread::hardware_concurrency(); // max number of threads

//////////////////////////////////////////////////////////////

/* Scene parameters */
const float aspect_ratio = {16.0 / 9.0};                               // Scene aspect ratio: resolution

int image_width = {RES_FULL_HD};                                       // Scene width
int image_height = {static_cast<int>(image_width / aspect_ratio)};     // Scene height

std::atomic<int> scenes_in_counter = {0};                              // currently renderd scenes

//////////////////////////////////////////////////////////////

/* Render parameters */
int samples_per_pixel = {1};                                           // rays per pixel
int max_depth = {2};                                                   // ray bounce limit per pixel

std::atomic<int> samples_in_counter = {0};                             //

//////////////////////////////////////////////////////////////

MyEmbree embree;

//////////////////////////////////////////////////////////////

/* Bucket parameters */
const int bucket_size = {64};                                          // bucket size x by y
int total_buckets = {0};                                               // total amount of buckets
std::atomic<int> buckets_in_counter = {0};                             // currently renderd buckets

//////////////////////////////////////////////////////////////

/* Image pixels */
std::vector<Color> pixels(image_width * image_height);                 // Scene output pixels
std::vector<int> samples_in_pixels(image_width * image_height);        // Scene samples in pixel

std::vector<Color> empty_pixels(image_width * image_height);           // Scene output pixels = (0.0f ,0.0f ,0.0f)
std::vector<int> empty_samples_in_pixels(image_width * image_height);  // Scene samples in pixel = 0 ... 0

//////////////////////////////////////////////////////////////

/* Background parameters */
Color background(0.0f, 0.0f, 0.0f);

//////////////////////////////////////////////////////////////

/* Camera parameters */
Point lookfrom(-2.0f, 1.0f, 2.0f);
Point lookat(0.0f, 1.0f, 0.0f);
Vec view_up(0.0f, 1.0f, 0.0f);

float fov = 53.7;
float aperture = 0.1f;
float dist_to_focus = 3.5f;

//////////////////////////////////////////////////////////////

/* Move parameters */
float precision = {0.1f};

//////////////////////////////////////////////////////////////

/* Event flags */
bool change_multithreading = false;
bool change_static         = false;
bool change_close_window   = false;
bool change_object_list    = false;
bool change_object         = false;
bool change_scene          = false;
bool change_camera         = false;
bool change_clear          = false;
bool change_default        = false;
bool change_edit_stop      = false;
bool change_force_stop     = true;

//////////////////////////////////////////////////////////////

/* Default settings */
int default_samples_per_pixel = {1};
int default_max_depth = {2};

float default_fov = 53.7;

Point default_lookfrom(0.0f, 1.0f, 0.0f);
Point default_lookat(1.0f, 1.0f, 0.0f);

Color default_background(0.0f, 0.0f, 0.0f);
