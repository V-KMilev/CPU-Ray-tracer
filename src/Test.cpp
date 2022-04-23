#include <iostream>
#include <fstream>
#include <ctime>

#include "GLFW_window_set.h"
#include "Log.h"

#include "Matrices.h"

std::stringstream* Logger::log = nullptr;

int main(int argc, char **argv) {

	std::cerr << "Ok we are working! For now..\n\n";

	Matrix a;
	Matrix b;
	Matrix d;
	Matrix c;
	Matrix c_i;

	a = get_scaling_matrix(5);
	b = get_scaling_matrix(4);

	std::cerr <<"a:\n" << a << '\n' << "b:\n" << b << '\n';

	c = a * b;

	std::cerr << "c:\n" << c << '\n';

	c_i = c.inverse_matrix();

	std::cerr << "c_i:\n" << c_i << '\n';

	c_i = c * c_i;

	std::cerr << "i:\n" << c_i << '\n';

	// window_setup();

	return 0;
}
