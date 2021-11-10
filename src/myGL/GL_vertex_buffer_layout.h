#pragma once

#include <vector>
#include <assert.h>

#include "GL_error_handler.h"

struct VectexBufferElement {

	unsigned int type;           // GLenum
	unsigned int count;          // GLuint
	unsigned char normalized;    // GLboolean
};

class VertexBufferLayout {
	public:
		VertexBufferLayout() : my_Stride(0) {}

		~VertexBufferLayout() {}

		template<typename T>
		void push(unsigned int count) {

			assert(false);    // needs to be static_assert
		}

		inline const std::vector<VectexBufferElement>& GetElements() const { return my_Elements; }

		inline unsigned int getStride() const { return my_Stride; }

	private:
		std::vector<VectexBufferElement> my_Elements;

		unsigned int my_Stride;
};

template<>
void VertexBufferLayout::push<float>(unsigned int count) {

	VectexBufferElement element;
	element.type = GL_FLOAT;
	element.count = count;
	element.normalized = GL_FALSE;

	my_Elements.push_back(element);

	// Custom for every template : float => GLfloat
	my_Stride += count * sizeof(GLfloat);
}
