#include <iostream>

#include "Utility_functions.h"

const float E = 1e-3f;

int main() {

	for (float i = 0.5f; i < 1.1f; i+=0.01f) {

		std::cerr << "i = " << i << " clamp = " << clamp(i, 0.0f, 1.0f - E) << " | "
			<< static_cast<int>(256 * clamp(i, 0.0f, 1.0f - E)) << ' '
			<< static_cast<int>(256 * clamp(i, 0.0f, 1.0f - E)) << ' '
			<< static_cast<int>(256 * clamp(i, 0.0f, 1.0f - E)) << '\n';

		std::cerr << "i = " << i << " clamp = " << clamp(i, 0.0, 0.999) << " | "
			<< static_cast<int>(256 * clamp(i, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(i, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(i, 0.0, 0.999)) << '\n';
		
		std::cerr << "-------------------------------------\n";
	}
}
