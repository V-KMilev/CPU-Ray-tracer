#include <iostream>
#include <fstream>
#include <ctime>

#include "GLFW_window_set.h"
#include "Log.h"

std::stringstream* Logger::log = nullptr;

int main(int argc, char **argv) {

	window_setup();

	return 0;
}
// Can get the full code from https://github.com/V-KMilev/Ray-tracer.git