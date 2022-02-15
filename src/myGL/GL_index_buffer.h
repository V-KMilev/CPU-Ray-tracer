#pragma once

#include "GL_error_handler.h"

class IndexBuffer {
	public:

		/*
		 * Class - Constructor
		 *
		 * Parameters:
		 * [p] const unsigned int* data, 
		 * [p] unsigned int count
		 * 
		 * Use:
		 * By creating index-buffer we 
		 * generate a buffer with id = my_ID. 
		 * After generating we bind the buffer. 
		 * After binding we set the data.
		 * Supporting 32-bit data (unsigned int)
		 */

		IndexBuffer(const unsigned int* data, unsigned int count) : my_Count(count) {

			MY_GL_CHECK(glGenBuffers(1, &my_ID));
			MY_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_ID));
			MY_GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
		}

		/*
		 * Class - Deconstructor
		 *
		 * Parameters:
		 * none
		 * 
		 * Use:
		 * By de-creating index-buffer we 
		 * delete the last binded buffer.
		 */

		~IndexBuffer() {
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
			MY_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_ID));
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
		 * the 0 buffer = none (free mem.).
		 */

		void unbind() const {
			MY_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}

		/*
		 * Function - getCount
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * unsigned int
		 * 
		 * Use:
		 * By calling getCount we get 
		 * in return the count of indices 
		 * we have.
		 */

		inline unsigned int getCount() { return my_Count; }

	private:
		unsigned int my_ID;
		unsigned int my_Count;
};
