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
		void setCtrl() {
			int state = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);

			if (state == GLFW_PRESS) {

			}
		}

		void setW(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_W);

			if (state == GLFW_PRESS) {
				samples_per_pixel    = default_samples_per_pixel;

				pixels               = empty_pixels;
				samples_in_pixels    = empty_samples_in_pixels;

				lookfrom[0] += precision;
				glfwWaitEventsTimeout(0.1);
			}
		}

		void setS(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_S);

			if (state == GLFW_PRESS) {
				samples_per_pixel    = default_samples_per_pixel;

				pixels               = empty_pixels;
				samples_in_pixels    = empty_samples_in_pixels;

				lookfrom[0] += -precision;
				glfwWaitEventsTimeout(0.1);
			}
		}

		void setQ(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_Q);

			if (state == GLFW_PRESS) {
				samples_per_pixel    = default_samples_per_pixel;

				pixels               = empty_pixels;
				samples_in_pixels    = empty_samples_in_pixels;

				lookfrom[1] += precision;
				glfwWaitEventsTimeout(0.1);
			}
		}

		void setE(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_E);

			if (state == GLFW_PRESS) {
				samples_per_pixel    = default_samples_per_pixel;

				pixels               = empty_pixels;
				samples_in_pixels    = empty_samples_in_pixels;
				lookfrom[1] += -precision;
				glfwWaitEventsTimeout(0.1);
			}
		}

		void setA(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_A);

			if (state == GLFW_PRESS) {
				samples_per_pixel    = default_samples_per_pixel;

				pixels               = empty_pixels;
				samples_in_pixels    = empty_samples_in_pixels;

				lookfrom[2] += -precision;
				glfwWaitEventsTimeout(0.1);
			}
		}

		void setD(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_D);

			if (state == GLFW_PRESS) {
				samples_per_pixel    = default_samples_per_pixel;

				pixels               = empty_pixels;
				samples_in_pixels    = empty_samples_in_pixels;

				lookfrom[2] += precision;
				glfwWaitEventsTimeout(0.1);
			}
		}

	private:
		GLFWwindow* window;
};
