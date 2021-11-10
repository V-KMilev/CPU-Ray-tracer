#pragma once

#include "GL_error_handler.h"

class IndexBuffer {
	public:
		// unsigned int - supporting 32-bit
		IndexBuffer(const unsigned int* data, unsigned int count) : my_Count(count) {

			MY_GL_CHECK(glGenBuffers(1, &my_ID));
			MY_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_ID));
			MY_GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
		}

		~IndexBuffer() {
			MY_GL_CHECK(glDeleteBuffers(1, &my_ID));
		}

		void bind() const {
			MY_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_ID));
		}

		void unbind() const {
			MY_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}

		inline unsigned int getCount() { return my_Count; }

	private:
		unsigned int my_ID;
		unsigned int my_Count;
};
