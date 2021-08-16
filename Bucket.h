#pragma once

#include <vector>

#include "Vec.h"

const int percent = 10;                                                                       // percent

const int buckets_size = percent * percent;                                                   // vector size
 
// bucket is percent of the image width x percent of the image height
struct Bucket {
    
    int start_x;                                                                              // Start index by x
    int start_y;                                                                              // Start index by y
    int end_x;                                                                                // End index by x
    int end_y;                                                                                // End index by y
};

std::vector<Bucket> bucket_segmentation(int image_width, int image_height) {

    const int height_percent = image_height/percent;                                          // percent of the image height
    const int width_percent = image_width/percent;                                            // percent of the image width 

    std::vector<Bucket> buckets (buckets_size);

    Bucket bucket;

    int counter = 0;

    for(int y = 0; y < image_height - height_percent; y += height_percent) {
        for (int x = 0; x < image_width - width_percent; x += width_percent) {
            
            bucket.start_x = x;
            bucket.start_y = y;

            if (counter % 10 == 9 && image_width != width_percent * percent) {
                bucket.end_x = x + (image_width - width_percent * (percent - 1));             // Get the rest of the pixel by width
            } else {
                bucket.end_x = x + width_percent;
            }

            if (counter >= 90 && image_height != height_percent * percent) {
                bucket.end_y = y + (image_height - height_percent * (percent -1));            // Get the rest of the pixel by height
            } else {
                bucket.end_y = y + height_percent;
            }

            buckets[counter++] = bucket;
        }
    }
    return buckets;
}
