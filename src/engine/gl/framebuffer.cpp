#include "framebuffer.hpp"

#include "../core/log.hpp"

namespace gl {

Framebuffer::Framebuffer()
  : m_obj{GL_FRAMEBUFFER}
{
	if (m_obj == 0) {
		LOG("Error creating Framebuffer!");
	}
}

Framebuffer::Framebuffer(const std::string & name)
  : m_obj{GL_FRAMEBUFFER}
{
	if (m_obj == 0) {
		LOG("Error creating Framebuffer " + name);
	} else {
		glObjectLabel(GL_FRAMEBUFFER, m_obj, static_cast<GLsizei>(name.size()), name.c_str());
	}
}

void Framebuffer::bind(const GLenum target) const {
	glBindFramebuffer(target, m_obj);
}

void Framebuffer::unbind(const GLenum target) const {
	glBindFramebuffer(target, 0);
}

GLint Framebuffer::getParameter(const GLenum pname) const {
	GLint ret;
	glGetNamedFramebufferParameteriv(m_obj, pname, &ret);
	return ret;
}

void Framebuffer::setParameter(const GLenum pname, const GLint param) const {
	glNamedFramebufferParameteri(m_obj, pname, param);
}

GLint Framebuffer::getAttachmentParameter(const GLenum attachment, const GLenum pname) const {
	GLint ret;
	glGetNamedFramebufferAttachmentParameteriv(m_obj, attachment, pname, &ret);
	return ret;
}

const glm::ivec2 Framebuffer::getAttachmentSize(const GLenum attachment) const {
	const auto type = getAttachmentParameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE);
	if (type != GL_TEXTURE) {
		LOG_WARNING("Framebuffer attachment is not a texture, can't read size!");
		return {-1, -1};
	}
	const auto name = getAttachmentParameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME);
	const auto level = getAttachmentParameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL);
	GLint width, height;
	glGetTextureLevelParameteriv(static_cast<GLuint>(name), level, GL_TEXTURE_WIDTH, &width);
	glGetTextureLevelParameteriv(static_cast<GLuint>(name), level, GL_TEXTURE_HEIGHT, &height);
	return {width, height};
}

void Framebuffer::attachTexture(const GLenum attachment, const gl::Texture & texture,
		const GLint level) const {
	glNamedFramebufferTexture(m_obj, attachment, texture, level);
}

bool Framebuffer::isComplete(const GLenum target) const {
	const auto e = glCheckNamedFramebufferStatus(m_obj, target);
	if (e == GL_FRAMEBUFFER_COMPLETE) {
		return true;
	} else {
		if (e == GL_FRAMEBUFFER_UNDEFINED) {
			LOG_WARNING("GL_FRAMEBUFFER_UNDEFINED");
		} else if (e == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
			LOG_WARNING("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
		} else if (e == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
			LOG_WARNING("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
		} else if (e == GL_FRAMEBUFFER_UNSUPPORTED) {
			LOG_WARNING("GL_FRAMEBUFFER_UNSUPPORTED");
		} else if (e == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE) {
			LOG_WARNING("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
		} else if (e == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS) {
			LOG_WARNING("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
		}
	}
	return false;
}

void Framebuffer::read(const GLenum src) const {
	glNamedFramebufferReadBuffer(m_obj, src);
}

void Framebuffer::draw(const std::vector<GLenum> & buffers) const {
	if (buffers.size() == 1) {
		glNamedFramebufferDrawBuffer(m_obj, buffers[0]);
	} else if (buffers.size() > 1) {
		glNamedFramebufferDrawBuffers(m_obj, static_cast<GLsizei>(buffers.size()), buffers.data());
	}
}

void Framebuffer::blit(const GLuint drawFramebuffer, const glm::ivec4 & src, const glm::ivec4 & dst,
		const GLbitfield mask, const GLenum filter) const {
	glBlitNamedFramebuffer(m_obj, drawFramebuffer, src.x, src.y, src.z, src.w, dst.x, dst.y,
			dst.z, dst.w, mask, filter);
}

void Framebuffer::blitAttachment(const GLenum src, const glm::ivec4 & dst) const {
	read(src);
	const auto size = getAttachmentSize(src);
	blit(0, {0, 0, size.x, size.y}, dst, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

} // namespace gl
