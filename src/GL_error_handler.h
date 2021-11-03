#pragma once

/* Future linking for glGenBuffers, glBindBuffer e.t. */
#define GL_GLEXT_PROTOTYPES

#include <GLFW/glfw3.h>

/* Printing funtion fail*/
static void printGLErrors(const char* file, int line, const char* fun) {

	GLenum err;

	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "[" << file << ":" << line << "][ERROR: " << err << "] my_gl_checking " << fun << "\n";
	}
}

/* do-while so we can execute MY_GL_CHECK, and we don't have to remove the ';' at the end of the function to which we my_gl_check MY_GL_CHECK*/
#define MY_GL_CHECK(MyFunction)                        \
do {                                                   \
	MyFunction;                                        \
	printGLErrors(__FILE__, __LINE__, #MyFunction);    \
} while(0)