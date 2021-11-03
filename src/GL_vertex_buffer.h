#pragma once

#include "GLFW_window_set.h"

class VertexBuffer {
	public:
		VertexBuffer(const void* data, unsigned int size) {
			
			MY_GL_CHECK(glGenBuffers(1, &my_ID));
			MY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, my_ID));
			MY_GL_CHECK(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
		}

		~VertexBuffer() {
			MY_GL_CHECK(glDeleteBuffers(1, &my_ID));
		}

		void bind() const {
			MY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, my_ID));
		}

		void unbind() const {
			MY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}

	private:
		unsigned int my_ID;
};