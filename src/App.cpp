#include <iostream>
#include <fstream>
#include <ctime>

#include "GLFW_window_set.h"
#include "Log.h"

#define EMBREE_IGNORE_INVALID_RAYS

std::stringstream* Logger::log = nullptr;

int main(int argc, char **argv) {

	std::cerr << "Ok we are working! For now..\n\n";

	window_setup();

	return 0;
}

// Can get the full code from https://github.com/V-KMilev/Ray-tracer.git