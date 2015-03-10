#pragma once

#include "globject.hpp"

#include <string>

namespace gl {

class VertexArray {

	public:

		VertexArray();
		VertexArray(const std::string &);
		VertexArray(const VertexArray &) = delete;
		VertexArray(VertexArray &&) = default;
		VertexArray & operator=(const VertexArray &) = delete;
		VertexArray & operator=(VertexArray &&) = default;
		~VertexArray() {}

		operator GLuint() const { return m_obj; }

		void bind() const;
		void unbind() const;

		bool attribBindingEnabled(unsigned int) const;
		void enableAttribBinding(unsigned int) const;

		// TO DO: auslagern
		void bindElementBuffer(GLuint) const;
		void bindVertexBuffer(unsigned int bindingIndex, GLuint buffer, unsigned int offset,
				unsigned int stride, unsigned int divisor = 0) const;

		void bindVertexFormat(unsigned int bindingIndex, unsigned int attribIndex, unsigned int size,
				GLenum type, bool normalized, unsigned int relativeOffset) const;

	private:

		GLObject m_obj;

};

} // namespace gl
