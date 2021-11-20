#pragma once

#ifdef _WIN32
	#include <windows.h>
#endif

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// W moves +1 by X, S moves -1 by X
// Q moces +1 by Y, E moves -1 by Y
// A moves +1 by Z, D moves -1 by Z

class MyGlfw {
	public:
		MyGlfw(GLFWwindow* window) : window(window) {}
		~MyGlfw() {}

		void fullControlSet(float precision) {
			setQ(precision); setW(precision); setE(precision);
			setA(precision); setS(precision); setD(precision);
		}

	private:
		void setW(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_W);

			if (state == GLFW_PRESS) {
				pixels = empty_pixels;
				lookfrom[0] += precision;
			}
		}

		void setS(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_S);

			if (state == GLFW_PRESS) {
				pixels = empty_pixels;
				lookfrom[0] += -precision;
			}
		}

		void setQ(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_Q);

			if (state == GLFW_PRESS) {
				pixels = empty_pixels;
				lookfrom[1] += precision;
			}
		}

		void setE(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_E);

			if (state == GLFW_PRESS) {
				pixels = empty_pixels;
				lookfrom[1] += -precision;
			}
		}

		void setA(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_A);

			if (state == GLFW_PRESS) {
				pixels = empty_pixels;
				lookfrom[2] += -precision;
			}
		}

		void setD(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_D);

			if (state == GLFW_PRESS) {
				pixels = empty_pixels;
				lookfrom[2] += precision;
			}
		}
	private:
		GLFWwindow* window;
};