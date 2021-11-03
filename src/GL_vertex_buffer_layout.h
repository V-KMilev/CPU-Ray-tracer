#pragma once

#include <vector>

#include "GL_error_handler.h"

struct VectexBufferElement {

	unsigned int count;          // GLuint
	unsigned int type;           // GLenum
	unsigned char normalized;    // GLboolean
};

class VertexBufferLayout {
	public:
		VertexBufferLayout() : my_Stride(0) {}

		~VertexBufferLayout() {}

		template<typename T>
		void push(unsigned int count) {

			static_assert(false);
		}

		template<>
		void push<float>(unsigned int count) {

			my_Elements.push_back( {GL_FLOAT, count, GL_FALSE} );
			my_Stride += count * sizeof(GLfloat);
		}

		inline const std::vector<VectexBufferElement>& GetElements() const { return my_Elements; }

		inline unsigned int getStride() const { return my_Stride; }

	private:
		std::vector<VectexBufferElement> my_Elements;

		unsigned int my_Stride;
};
