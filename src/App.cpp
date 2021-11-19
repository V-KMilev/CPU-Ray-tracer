#include <iostream>
#include <fstream>
#include <ctime>

#include "GLFW_window_set.h"

int main(int argc, char **argv) {
	
	for (auto it = spp.begin(); it != spp.end(); it++) {
		*it = 0;
	}

	window_setup();

	return 0;
}
