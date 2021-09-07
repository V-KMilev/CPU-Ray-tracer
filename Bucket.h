#pragma once

#include <vector>

const int bucket_size = { 64 };

struct Bucket {

	int start_x;                                                                                // Start index x
	int start_y;                                                                                // Start index y
	int end_x;                                                                                  // End index x
	int end_y;                                                                                  // End index y

	int bucket_id;                                                                              // Bucket id
};

std::vector<Bucket> bucket_segmentation(const int image_width, const int image_height) {

	const int columns = { image_width / bucket_size };
	const int rows = { image_height / bucket_size };

	const int leftover_column = { image_width - bucket_size * columns };
	const int leftover_row = { image_height - bucket_size * rows };

	const int numb_of_buckets = { rows * columns };

	std::vector<Bucket> my_buckets(numb_of_buckets);

	int bucket_idx = { 0 };
	int row_idx = { 0 };

	for (int y = 0; y < image_height - bucket_size; y+=bucket_size, row_idx++) {
		for (int x = 0; x < image_width - bucket_size; x+=bucket_size) {

			my_buckets[bucket_idx].bucket_id = bucket_idx;
			
			my_buckets[bucket_idx].start_x = x;
			my_buckets[bucket_idx].start_y = y;

			if (bucket_idx == row_idx * columns + (columns - 1)) {
				my_buckets[bucket_idx].end_x = x + bucket_size + leftover_column;
			}
			else {
				my_buckets[bucket_idx].end_x = x + bucket_size;
			}

			if(bucket_idx > numb_of_buckets - columns - 1) {
				my_buckets[bucket_idx].end_y = y + bucket_size + leftover_row;
			} else {
				my_buckets[bucket_idx].end_y = y + bucket_size;
			}
			bucket_idx++;
		}
	}
	return my_buckets;
}
