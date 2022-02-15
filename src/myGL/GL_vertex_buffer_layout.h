#pragma once

#include <vector>
#include <assert.h>

#include "GL_error_handler.h"

struct VertexBufferElement {

	unsigned int type;           // GLenum
	unsigned int count;          // GLuint
	unsigned char normalized;    // GLboolean
};

class VertexBufferLayout {
	public:
		VertexBufferLayout() : my_Stride(0) {}

		~VertexBufferLayout() {}

		/*
		 * Function - push
		 *
		 * Parameters:
		 * [p] unsigned int count
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * templated function. 
		 * By calling push we add new 
		 * vertex buffer element specifications.
		 */

		template<typename T>
		void push(unsigned int count) {

			assert(false);    // needs to be static_assert
		}

		/*
		 * Function - getElements
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * const std::vector<VertexBufferElement>&
		 * 
		 * Use:
		 * By calling getElements we 
		 * get in return the vector with 
		 * all elements.
		 */

		inline const std::vector<VertexBufferElement>& getElements() const { return my_Elements; }

		/*
		 * Function - getStride
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * unsigned int
		 * 
		 * Use:
		 * By calling getStride we 
		 * get in return the stride count.
		 */

		inline unsigned int getStride() const { return my_Stride; }

	private:
		std::vector<VertexBufferElement> my_Elements;

		unsigned int my_Stride;
};

template<>
void VertexBufferLayout::push<float>(unsigned int count) {

	VertexBufferElement element;
	element.type = GL_FLOAT;
	element.count = count;
	element.normalized = GL_FALSE;

	my_Elements.push_back(element);

	/* Custom for every template : float == GLfloat */
	my_Stride += count * sizeof(GLfloat);
}
