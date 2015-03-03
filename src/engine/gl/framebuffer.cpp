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

GLint Framebuffer::getParameter(const GLenum target, const GLenum pname) const {
	GLint ret;
	glGetFramebufferParameteriv(target, pname, &ret);
	return ret;
}

void Framebuffer::setParameter(const GLenum target, const GLenum pname, const GLint param) const {
	glFramebufferParameteri(target, pname, param);
}

GLint Framebuffer::getAttachmentParameter(const GLenum target, const GLenum attachment,
		const GLenum pname) const {
	GLint ret;
	glGetFramebufferAttachmentParameteriv(target, attachment, pname, &ret);
	return ret;
}

const glm::ivec2 Framebuffer::getAttachmentSize(const GLenum target, const GLenum attachment) const {
	const auto type = getAttachmentParameter(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE);
	if (type != GL_TEXTURE) {
		LOG_WARNING("Framebuffer attachment is not a texture, can't read size!");
		return {-1, -1};
	}
	const auto name = getAttachmentParameter(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME);
	glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(name));
	const auto level = getAttachmentParameter(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL);
	GLint width, height;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &height);
	glBindTexture(GL_TEXTURE_2D, 0);
	return {width, height};
}

void Framebuffer::attachTexture(const GLenum target, const GLenum attachment, const gl::Texture & texture,
		const GLint level) const {
	glFramebufferTexture(target, attachment, texture, level);
}

bool Framebuffer::isComplete(const GLenum target) const {
	const auto e = glCheckFramebufferStatus(target);
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
	glReadBuffer(src);
}

void Framebuffer::draw(const std::vector<GLenum> & buffers) const {
	if (buffers.size() == 1) {
		glDrawBuffer(buffers[0]);
	} else if (buffers.size() > 1) {
		glDrawBuffers(static_cast<GLsizei>(buffers.size()), buffers.data());
	}
}

void Framebuffer::blit(const glm::ivec4 & src, const glm::ivec4 & dst,
		const GLbitfield mask, const GLenum filter) const {
	glBlitFramebuffer(src.x, src.y, src.z, src.w, dst.x, dst.y,
			dst.z, dst.w, mask, filter);
}

void Framebuffer::blitAttachment(const GLenum target, const GLenum src, const glm::ivec4 & dst) const {
	read(src);
	const auto size = getAttachmentSize(target, src);
	blit({0, 0, size.x, size.y}, dst, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

} // namespace gl
