#include <iostream>
#include <fstream>

#include "Utility_functions.h"

#include "Hittable_list.h"
#include "Sphere.h"
#include "Camera.h"

#include "Hittable.h"

#include "Color.h"
#include "Vec.h"
#include "Ray.h"

Color ray_color(const Ray& ray, const Hittable& world) {
    hit_record record;
    if (world.hit(ray, 0, infinity, record)) {
        return 0.5 * (record.normal + Color(1.0,1.0,1.0));
    }

    Vec unit_direction = unit_vector(ray.getDirection());
    
    float distance = 0.5 * (unit_direction.getY() + 1.0);
    return (1.0 - distance) * Color(1.0, 0.0, 0.0) + distance * Color(0.0, 0.0, 0.0);
}

int main() {

    // Image
    const float aspect_ratio = 16.0 / 9.0;                                                                      // Image: Aspect ratio: resolution
    const int image_width = 1000;                                                                               // Image: Width
    const int image_height = static_cast<int>(image_width / aspect_ratio);                                      // Image: Height
    const int samples_per_pixel = 100;

    // World
    Hittable_list world;
    world.add(make_shared<Sphere>(Point(0,0,-1), 0.5));
    world.add(make_shared<Sphere>(Point(0,-100.5,-1), 100));

    // Camera
    Camera camera;

    std::ofstream out("RTout.ppm");

    out << "P3\n" << image_width << " " << image_height << "\n255\n";

    std::cerr << "\rWriting...\n";
    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {
           
            Color pixel_color(0, 0, 0);
           
            for (int s = 0; s < samples_per_pixel; ++s) {
             
                float u = (x + random_float()) / (image_width  - 1);
                float v = (y + random_float()) / (image_height - 1);
                
                Ray ray = camera.get_Ray(u, v);
                
                pixel_color += ray_color(ray, world);
            }
            write_color(out, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\rDone!\n";
    out.close();
}