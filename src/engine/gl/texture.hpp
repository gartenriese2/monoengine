#pragma once

#include "globject.hpp"

namespace gl {

class Texture {

	public:

		Texture();
		Texture(const std::string & name);
		Texture(unsigned int width, unsigned int height, GLenum internalFormat = GL_RGBA,
				unsigned int levels = 1);
		Texture(const std::string & name, unsigned int width, unsigned int height,
				GLenum internalFormat = GL_RGBA, unsigned int levels = 1);
		Texture(const Texture &) = delete;
		Texture(Texture &&) = default;
		Texture & operator=(const Texture &) = delete;
		Texture & operator=(Texture &&) & = default;
		~Texture() {}

		operator GLuint() const noexcept { return m_obj; }
		GLuint * getHandlePtr() noexcept { return m_obj.getHandlePtr(); } // imgui needs this

		void createImmutableStorage(unsigned int width, unsigned int height, GLenum internalFormat,
				unsigned int levels = 1) const;
		void fill(const void *) const; // TO DO
		void fillSubImage(unsigned int level, unsigned int xoffset, unsigned int yoffset,
				unsigned int width, unsigned int height, GLenum format, GLenum type,
				const void * pixels) const;

		void bindToTextureUnit(unsigned int unit) const;
		void setParameter(GLenum pname, GLfloat param) const;
		void setParameter(GLenum pname, GLint param) const;

	private:

		GLObject m_obj;

};

static_assert(std::is_nothrow_move_constructible<Texture>(), "Should be noexcept MoveConstructible");
static_assert(!std::is_copy_constructible<Texture>(), "Should not be CopyConstructible");
static_assert(std::is_nothrow_move_assignable<Texture>(), "Should be noexcept MoveAssignable");
static_assert(!std::is_copy_assignable<Texture>(), "Should not be CopyAssignable");

} // namespace gl
