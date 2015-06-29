#pragma once

#include "globject.hpp"

#include <string>

namespace gl {

class VertexArray {

	public:

		VertexArray();
		VertexArray(const std::string & name);
		VertexArray(const VertexArray &) = delete;
		VertexArray(VertexArray &&) = default;
		VertexArray & operator=(const VertexArray &) = delete;
		VertexArray & operator=(VertexArray &&) & = default;
		~VertexArray() {}

		operator GLuint() const noexcept { return m_obj; }

		void bind() const;
		void unbind() const;

		bool attribBindingEnabled(unsigned int attribIndex) const;
		void enableAttribBinding(unsigned int bindingIndex) const;

		// TO DO: auslagern
		void bindElementBuffer(GLuint buffer) const;
		void bindVertexBuffer(unsigned int bindingIndex, GLuint buffer, unsigned int offset,
				unsigned int stride, unsigned int divisor = 0) const;

		void bindVertexFormat(unsigned int bindingIndex, unsigned int attribIndex, unsigned int size,
				GLenum type, bool normalized, unsigned int relativeOffset) const;

	private:

		GLObject m_obj;

};

static_assert(std::is_nothrow_move_constructible<VertexArray>(), "Should be noexcept MoveConstructible");
static_assert(!std::is_copy_constructible<VertexArray>(), "Should not be CopyConstructible");
static_assert(std::is_nothrow_move_assignable<VertexArray>(), "Should be noexcept MoveAssignable");
static_assert(!std::is_copy_assignable<VertexArray>(), "Should not be CopyAssignable");

} // namespace gl
