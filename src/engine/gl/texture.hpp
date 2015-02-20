#pragma once

#include "globject.hpp"

namespace gl {

class Texture {

	public:

		Texture();
		Texture(const std::string &);
		Texture(unsigned int, unsigned int, GLenum = GL_RGBA, unsigned int = 1);
		Texture(const std::string &, unsigned int, unsigned int, GLenum = GL_RGBA, unsigned int = 1);
		Texture(const Texture &) = delete;
		Texture(Texture &&) = default;
		Texture & operator=(const Texture &) = delete;
		Texture & operator=(Texture &&) = default;
		~Texture() {}

		operator GLuint() const { return m_obj; }
		GLuint * getHandlePtr() { return m_obj.getHandlePtr(); } // imgui needs this

		void createImmutableStorage(unsigned int, unsigned int, GLenum, unsigned int = 1) const;
		void fill(const void *) const; // TO DO
		void fillSubImage(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int,
				GLenum, GLenum, const void *) const;

		void bindToTextureUnit(unsigned int) const;
		void setParameter(GLenum, GLfloat) const;
		void setParameter(GLenum, GLint) const;

	private:

		GLObject m_obj;

};

} // namespace gl
