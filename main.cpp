#include <iostream>
#include <iomanip> 
#include <fstream>
#include <ctime>

#include "Utility_functions.h"

#include "Hittable_list.h"
#include "Hittable.h"

#include "Material.h"
#include "Sphere.h"

#include "Camera.h"

#include "Color.h"
#include "Vec.h"
#include "Ray.h"

Color ray_color(const Ray &ray, const Hittable &world, int depth) {
    hit_record record;

     // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
        return Color(0,0,0);                                                                                    // Set current pixel value to black
    }

    if (world.hit(ray, 0.001, infinity, record)) {                                                              // 0.001 to fix shadow problem
    
        Ray scattered;
        Color attenuation;

        if (record.material_ptr -> scatter(ray, record, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }

        return Color(0,0,0);
    }

    Vec unit_direction = unit_vector(ray.getDirection());

    float distance = 0.5 * (unit_direction.getY() + 1.0);
    return (1.0 - distance) * Color(1.0, 0.0, 0.0) + distance * Color(0.5, 0.7, 1.0);                           // Blend Value
}

int main() {

    clock_t start_time = clock();

    time_t now_s = time(0);
   
    // convert now to string form
    char* dt = ctime(&now_s);

    std::cerr << "\rStart time: " << dt;


    // Image: 1.5 in full image width
    // Image: 1 is full image heigth
    // IMAGE:
    const float aspect_ratio = 16.0 / 9.0;                                                                      // Image: Aspect ratio: resolution
    const int image_width = 100;                                                                               // Image: Width
    const int image_height = static_cast<int>(image_width / aspect_ratio);                                      // Image: Height
    
    const int samples_per_pixel = 500;                                                                          // Rays per pixel
    const int max_depth = 100;                                                                                  // Ray bounce limit per pixel

    // World set: +x goes right from the camera
    // World set: +y goes down from the camera
    // World set: +z goes behind from the camera
    // WOLRD left hand:
    Hittable_list world;

    shared_ptr<Material> material_sphere_g = make_shared<Lambertian>(Color(0.2, 0.2, 0.2));
    shared_ptr<Material> material_sphere_c = make_shared<Glass>(Color(0.8, 0.8, 0.8));
    shared_ptr<Material> material_sphere_m = make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);

    for (int i = 0; i < 100; i++) {

        shared_ptr<Material> material_sphere_l = make_shared<Lambertian>(Color(random_float(0, 1), random_float(0, 1), random_float(0, 1)));

        if(i % 4 == 0 || i % 3 == 0) {
            world.add(make_shared<Sphere>
            (
                Point(random_float(-30, 30), random_float(0, -20), random_float(5, -30)),
                random_float(0.5, 2.5),
                material_sphere_l
            ));

        } else if(i % 5 == 0 || i % 7 == 0) {
            world.add(make_shared<Sphere>
            (
                Point(random_float(-30, 30), random_float(0, -20), random_float(5, -30)),
                random_float(0.5, 2.5),
                material_sphere_m
            ));
        } else {
            world.add(make_shared<Sphere>
            (
                Point(random_float(-30, 30), random_float(0, -20), random_float(5, -30)),
                random_float(0.5, 2.5),
                material_sphere_c
            ));
        }
    }

    world.add(make_shared<Sphere>(Point(0.0, 300.5, -1.0), 300.0, material_sphere_g));

    // CAMERA: World rules applied
    Camera camera(Point(7.0, -5.0, 13.0), Point(0.0, 0.0, -1.0), Vec(0.0, 1.0, 0.0), 90.0, aspect_ratio);

    std::ofstream out("RTout.ppm");

    out << "P3\n" << image_width << " " << image_height << "\n255\n";

    std::cerr << "\n\rWriting...\n";

    for (int y = 0; y < image_height; y++) {

        if(y % 10 == 0) { std::cerr << "\rDone: " << y * 100 / image_height << "%"; }

        for (int x = 0; x < image_width; x++) {
           
            Color pixel_color(0, 0, 0);
           
            for (int s = 0; s < samples_per_pixel; s++) {
             
                float u = (x + random_float()) / (image_width  - 1);
                float v = (y + random_float()) / (image_height - 1);
                
                Ray ray = camera.get_Ray(u, v);
                
                pixel_color += ray_color(ray, world, max_depth);
            }
            write_color(out, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\rDone! 100%";
    out.close();

    clock_t end_time = clock();

    std::cerr << std::fixed << "\n\rTime: " << std::setprecision(2) << ((end_time - start_time) / CLOCKS_PER_SEC) / 3600.0 << " h.";

    time_t now_e = time(0);

    // convert now to string form
    dt = ctime(&now_e);

    std::cerr << "\n\n\rEnd time: " << dt;

}
// 7680