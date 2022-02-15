#pragma once

#include "GL_error_handler.h"

class VertexBuffer {
	public:

		/*
		 * Class - Constructor
		 *
		 * Parameters:
		 * [p] const void* data,
		 * [p] unsigned int size
		 * 
		 * Use:
		 * Generates buffer, binds it and
		 * set the data into it.
		 */

		VertexBuffer(const void* data, unsigned int size) {

			MY_GL_CHECK(glGenBuffers(1, &my_ID));
			MY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, my_ID));
			MY_GL_CHECK(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
		}

		/*
		 * Class - Deconstructor
		 *
		 * Parameters:
		 * none
		 * 
		 * Use:
		 * By de-creating vertex-buffer we 
		 * delete the last binded buffer.
		 */

		~VertexBuffer() {
			MY_GL_CHECK(glDeleteBuffers(1, &my_ID));
		}

		/*
		 * Function - bind
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling bind we bind 
		 * the buffer with the corresponding id.
		 */

		void bind() const {
			MY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, my_ID));
		}

		/*
		 * Function - unbind
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling unbind we bind 
		 * the 0 buffer = none (free mem.).
		 */

		void unbind() const {
			MY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}

	private:
		unsigned int my_ID;
};
