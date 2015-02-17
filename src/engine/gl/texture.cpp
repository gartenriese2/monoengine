#include "texture.hpp"

#include "../core/log.hpp"

namespace gl {

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

} // namespace gl
