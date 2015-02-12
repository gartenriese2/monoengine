#pragma once

#include "globject.hpp"

namespace gl {

class Texture {

	public:

		Texture(unsigned int, unsigned int, GLenum = GL_RGBA, unsigned int = 1);
		Texture(const std::string &, unsigned int, unsigned int, GLenum = GL_RGBA, unsigned int = 1);
		Texture(const Texture &) = delete;
		Texture(Texture &&) = default;
		Texture & operator=(const Texture &) = delete;
		Texture & operator=(Texture &&) = default;
		~Texture() {}

	private:

		GLObject m_obj;

};

} // namespace gl
