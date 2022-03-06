#include <iostream>
#include <fstream>
#include <ctime>

#include "GLFW_window_set.h"
#include "Log.h"

#include "Matrices.h"

std::stringstream* Logger::log = nullptr;

int main(int argc, char **argv) {

	std::cerr << "Ok we are working! For now..\n\n";

	float degree = 180;
	float rad = degrees_to_radians(degree);

	float m_cos = cos(rad);
	float m_sin = sin(rad);

	float x,y,z;
	x = 2;
	y = 3;
	z = 4;

	float rot_y[9] = {
		m_cos, 0.f, m_sin,
		0.f, 1.f, 0.f,
		-m_sin, 0.f, m_cos
	};

	float scale[9] = {
		x, 0.f, 0.f,
		0.f, y, 0.f,
		0.f, 0.f, z
	};

	float ind[9] = {
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f
	};

	Matrix a;
	Matrix b;
	Matrix c;

	a = scale;
	b = scale;

	std::cerr << a.get_det() << '\n' << a << '\n';

	a.inverse_matrix();

	std::cerr << a << '\n';
	
	a = a * b;

	std::cerr << a;

	// window_setup();

	return 0;
}
