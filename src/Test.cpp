#include <iostream>

#include "GLFW_window_set.h"
#include "Log.h"

const float E = 1e-3f;

std::stringstream* Logger::log = nullptr;

int main(int argc, char **argv) {

	window_setup();
	
	return 0;
}
