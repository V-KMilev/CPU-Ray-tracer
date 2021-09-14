#pragma once

#include "Utility_functions.h"
#include "Ray.h"
#include "Vec.h"

class AABB {
	public:
		AABB() {}
		AABB(const Point &min, const Point &max) { minimum = min; maximum = max;}

		Point min() const {return minimum; }
		Point max() const {return maximum; }

		bool hit(const Ray &ray, float distance_min, float distance_max) const {
			for (int a = 0; a < 3; a++) {
				float f_min = fmin(
					(minimum[a] - ray.get_origin()[a]) / ray.get_direction()[a],
					(maximum[a] - ray.get_origin()[a]) / ray.get_direction()[a]);
				float f_max = fmax(
					(minimum[a] - ray.get_origin()[a]) / ray.get_direction()[a],
					(maximum[a] - ray.get_origin()[a]) / ray.get_direction()[a]);
				
				distance_min = fmax(f_min, distance_min);
				distance_max = fmin(f_max, distance_max);
				
				if (distance_max <= distance_min) { return false; }
			}
			return true;
		}

	public:
		Point minimum;
		Point maximum;
};

AABB surrounding_box(AABB box_0, AABB box_1) {
	
	Point small(
		fmin(box_0.min().getX(), box_1.min().getX()),
		fmin(box_0.min().getY(), box_1.min().getY()),
		fmin(box_0.min().getZ(), box_1.min().getZ())
	);

	Point big(
		fmax(box_0.max().getX(), box_1.max().getX()),
		fmax(box_0.max().getY(), box_1.max().getY()),
		fmax(box_0.max().getZ(), box_1.max().getZ())
	);

	return AABB(small,big);
}