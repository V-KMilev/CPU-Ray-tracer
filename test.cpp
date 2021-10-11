#include <iostream>

#include "Utility_functions.h"

int main() {

	for (int i = -10; i < 10; i++) {
		for (int j = -10; j < 10; j++) {
				std::cerr << i * random_float(1,2) * 10 << " " << j * random_float(1,2) * 10 << "\n";
		}
	}
}