#include "texture.hpp"

#include "../core/log.hpp"

namespace gl {

Texture::Texture()
  : m_obj{GL_TEXTURE_2D}
{
	if (m_obj == 0) {
		LOG("Error creating Texture!");
		return;
	}
}

Texture::Texture(const std::string & name)
  : m_obj{GL_TEXTURE_2D}
{
	if (m_obj == 0) {
		LOG("Error creating Texture " + name);
		return;
	}
	glObjectLabel(GL_TEXTURE, m_obj, static_cast<GLsizei>(name.size()), name.c_str());
}

Texture::Texture(const unsigned int width, const unsigned int height,
		const GLenum internalFormat, const unsigned int levels)
  : m_obj{GL_TEXTURE_2D}
{
	if (m_obj == 0) {
		LOG("Error creating Texture!");
		return;
	}
	glTextureStorage2D(m_obj, static_cast<GLsizei>(levels), internalFormat,
			static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

Texture::Texture(const std::string & name, const unsigned int width, const unsigned int height,
		const GLenum internalFormat, const unsigned int levels)
  : m_obj{GL_TEXTURE_2D}
{
	if (m_obj == 0) {
		LOG("Error creating Texture " + name);
		return;
	}

	glObjectLabel(GL_TEXTURE, m_obj, static_cast<GLsizei>(name.size()), name.c_str());
	glTextureStorage2D(m_obj, static_cast<GLsizei>(levels), internalFormat,
			static_cast<GLsizei>(width), static_cast<GLsizei>(height));

}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, m_obj);
}

void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::createImmutableStorage(const unsigned int width, const unsigned int height,
		const GLenum internalFormat, const unsigned int levels) const {
	glTexStorage2D(GL_TEXTURE_2D, static_cast<GLsizei>(levels), internalFormat,
			static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void Texture::fillSubImage(const unsigned int level, const unsigned int xoffset, const unsigned int yoffset,
		const unsigned int width, const unsigned int height, const GLenum format, const GLenum type,
		const void * pixels) const {
	glTexSubImage2D(GL_TEXTURE_2D, static_cast<GLint>(level), static_cast<GLint>(xoffset),
			static_cast<GLint>(yoffset), static_cast<GLsizei>(width), static_cast<GLsizei>(height),
			format, type, pixels);
}

void Texture::bindToTextureUnit(const unsigned int unit) const {
	glActiveTexture(GL_TEXTURE0 + unit);
	bind();
}

void Texture::setParameter(const GLenum pname, const GLfloat param) const {
	glTexParameterf(GL_TEXTURE_2D, pname, param);
}

void Texture::setParameter(const GLenum pname, const GLint param) const {
	glTexParameteri(GL_TEXTURE_2D, pname, param);
}

} // namespace gl
