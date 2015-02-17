#pragma once

#include "../extern/glinclude.hpp"

#include <functional>

namespace gl {

class GLObject {

	public:

		GLObject();
		GLObject(GLenum);
		GLObject(const GLObject &) = delete;
		GLObject(GLObject &&) = default;
		GLObject & operator=(const GLObject &) = delete;
		GLObject & operator=(GLObject &&) = default;
		~GLObject();

		operator GLuint() const { return m_handle; }

	private:

		GLuint m_handle;
		std::function<void(GLuint &)> m_deleter;

};

} // namespace gl
