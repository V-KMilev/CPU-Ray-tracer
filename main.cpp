#include <iostream>
#include <fstream>

#include "Vec.h"
#include "Ray.h"

float hit_sphere(const Point& center, float radius, const Ray& r) {

    Vec oc = r.getOrigin() - center;                                                                            //
    
    float a = r.getDirection().length_squared();                                                                // Discriminant: a
    float half_b = dot(oc, r.getDirection());                                                                   // Discriminant: half b
    float c = oc.length_squared() - radius * radius;                                                            // Discriminant: c

    float discriminant = half_b * half_b - a * c;                                                               // Discriminant
    
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;                                                             // Quadratic equation
    }
}

Color ray_color(const Ray& r) {

    float t = hit_sphere(Point(0,0,-1), 0.5, r);
    
    if (t > 0.0) {
        Vec N = unit_vector(r.at(t) - Vec(0,0,-1));

        return 0.5 * Color(N.getX() + 1, N.getY() + 1, N.getZ() + 1);
    }

    Vec unit_direction = unit_vector(r.getDirection());
    
    t = 0.5 * (unit_direction.getY() + 1.0);

    return (1.0 - t) * Color(1.0, 0.0, 0.0) + t * Color(0.0, 0.0, 0.0);                                         // blend value
}

int main() {

    // Image
    const float aspect_ratio = 16.0 / 9.0;                                                                      // Image: Aspect ratio (resolution)
    const int image_width = 1000;                                                                               // Image: Width
    const int image_height = static_cast<int>(image_width / aspect_ratio);                                      // Image: Height

    // Camera
    const float viewport_width = image_width / 2;                                                               // Camera: veiw point Width
    const float viewport_height = image_height / 2;                                                             // Camera: veiw point Height
    const float focal_length = 1.0;                                                                             // Camera: focal length

    const Point origin = Point(0, 0, 0);                                                                        // Camera: original point

    const Vec horizontal = Vec(viewport_width, 0, 0);                                                           // Camera: horizontal
    const Vec vertical = Vec(0, viewport_height, 0);                                                            // Camera: vertical
    
    const Point lower_left_corner = origin + Vec(0, 0, focal_length) - horizontal / 2 - vertical / 2;           // Camera: left coner

    std::ofstream out("RTout.ppm");

    out << "P3\n" << image_width << " " << image_height << "\n255\n";

    std::cerr << "\rWriting...\n";
    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {
            
            float ray_img_x = (float) x / image_width;                                                          // Ray: image x positon
            float ray_img_y = (float) y / image_height;                                                         // Ray: image y postion

            Ray r
            (
                origin,
                lower_left_corner + ray_img_x * horizontal + ray_img_y * vertical - origin
            );

            Color pixel_color = ray_color(r);

            write_color(out, pixel_color);
        }
    }

    std::cerr << "\rDone!\n";
    out.close();
}