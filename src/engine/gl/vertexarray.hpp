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

		bool attribBindingEnabled(unsigned int) const;
		void enableAttribBinding(unsigned int) const;

		// TO DO: auslagern
		void bindElementBuffer(GLuint) const;
		void bindVertexBuffer(GLuint, unsigned int, unsigned int,
				unsigned int, unsigned int) const;

		void bindVertexFormat(unsigned int, unsigned int, GLenum, bool, unsigned int) const;

	private:

		GLObject m_obj;

};

} // namespace gl
