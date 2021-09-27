#pragma once

#include "Utility_functions.h"
#include "Vec.h"

class Perlin {
	public:
		Perlin() {
			ranfloat = new float[point_count];
			
			for (int i = 0; i < point_count; ++i) {
				ranfloat[i] = random_float();
			}

			perm_x = perlin_generate_perm();
			perm_y = perlin_generate_perm();
			perm_z = perlin_generate_perm();
		}

		~Perlin() {
			delete[] ranfloat;
			delete[] perm_x;
			delete[] perm_y;
			delete[] perm_z;
		}

		float noise(const Point &point) const {
			int i = static_cast<int>(4 * point.getX()) & 255;
			int j = static_cast<int>(4 * point.getY()) & 255;
			int k = static_cast<int>(4 * point.getZ()) & 255;

			return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
		}
	private:
		static int* perlin_generate_perm() {
			int* perm = new int[point_count];

			for (int i = 0; i < Perlin::point_count; i++) {
				perm[i] = i;
			}

			permute(perm, point_count);

			return perm;
		}

		static void permute(int* perm, int number) {
			for (int i = number-1; i > 0; i--) {
				
				int target = random_int(0, i);
				int tmp = perm[i];
				
				perm[i] = perm[target];
				perm[target] = tmp;
			}
		}

	private:
		static const int point_count = 256;
		
		int* perm_x;
		int* perm_y;
		int* perm_z;
		
		float* ranfloat;
};
