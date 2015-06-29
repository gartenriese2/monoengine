#pragma once

#include "../extern/glinclude.hpp"

#include <functional>

namespace gl {

class GLObject {

	public:

		GLObject();
		GLObject(GLenum);
		GLObject(const GLObject &) = delete;
		GLObject(GLObject && other) noexcept;
		GLObject & operator=(const GLObject &) = delete;
		GLObject & operator=(GLObject && other) & noexcept;
		~GLObject();

		operator GLuint() const noexcept { return m_handle; }
		GLuint * getHandlePtr() noexcept { return &m_handle; } // imgui needs this

	private:

		GLuint m_handle;
		std::function<void(GLuint &)> m_deleter;

};

static_assert(std::is_nothrow_move_constructible<GLObject>(), "Should be noexcept MoveConstructible");
static_assert(!std::is_copy_constructible<GLObject>(), "Should not be CopyConstructible");
static_assert(std::is_nothrow_move_assignable<GLObject>(), "Should be noexcept MoveAssignable");
static_assert(!std::is_copy_assignable<GLObject>(), "Should not be CopyAssignable");

} // namespace gl
