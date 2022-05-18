#pragma once

#ifdef _WIN32
	#include <windows.h>
#endif

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

// W, UP moves +1 by X, S, DOWN moves -1 by X
// A, LEFT moves +1 by Z, D, RIGHT moves -1 by Z
// Q moces +1 by Y, E moves -1 by Y

class MyGlfw {
	public:
		MyGlfw(GLFWwindow* window) : window(window) {}
		~MyGlfw() {}

		void fullControlSet(float precision) {
			setYp(precision); setXp(precision); setYn(precision);
			setZp(precision); setXn(precision); setZn(precision);
		}

	private:
		void setCtrl() {
			int state = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);

			if(state == GLFW_PRESS) {

			}
		}

		void setXp(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP);

			if(state == GLFW_PRESS) {
				change_camera = true;
				change_edit_stop = true;

				lookfrom[0] += precision;
				glfwWaitEventsTimeout(0.7);
			}
		}

		void setXn(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN);

			if(state == GLFW_PRESS) {
				change_camera = true;
				change_edit_stop = true;

				lookfrom[0] += -precision;
				glfwWaitEventsTimeout(0.7);
			}
		}

		void setYp(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_Q);

			if(state == GLFW_PRESS) {
				change_camera = true;
				change_edit_stop = true;

				lookfrom[1] += precision;
				glfwWaitEventsTimeout(0.7);
			}
		}

		void setYn(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_E);

			if(state == GLFW_PRESS) {
				change_camera = true;
				change_edit_stop = true;

				lookfrom[1] += -precision;
				glfwWaitEventsTimeout(0.7);
			}
		}

		void setZp(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT);

			if(state == GLFW_PRESS) {
				change_camera = true;
				change_edit_stop = true;

				lookfrom[2] += precision;
				glfwWaitEventsTimeout(0.7);
			}
		}

		void setZn(float precision) {
			int state = glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT);

			if(state == GLFW_PRESS) {
				change_camera = true;
				change_edit_stop = true;

				lookfrom[2] += -precision;
				glfwWaitEventsTimeout(0.7);
			}
		}

	private:
		GLFWwindow* window;
};
