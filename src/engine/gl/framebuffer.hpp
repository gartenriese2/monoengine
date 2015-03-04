#pragma once

#include "globject.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace gl {

class Framebuffer {

	public:

		Framebuffer();
		Framebuffer(const std::string &);
		Framebuffer(const Framebuffer &) = delete;
		Framebuffer(Framebuffer &&) = default;
		Framebuffer & operator=(const Framebuffer &) = delete;
		Framebuffer & operator=(Framebuffer &&) = default;
		~Framebuffer() {}

		operator GLuint() const { return m_obj; }

		void bind(GLenum = GL_FRAMEBUFFER) const;
		void unbind(GLenum = GL_FRAMEBUFFER) const;

		GLint getParameter(GLenum) const;
		void setParameter(GLenum, GLint) const;
		GLint getAttachmentParameter(GLenum, GLenum) const;
		const glm::ivec2 getAttachmentSize(GLenum) const;

		void attachTexture(GLenum, const gl::Texture &, GLint) const;

		bool isComplete() const;

		void read(GLenum) const;
		void draw(const std::vector<GLenum> &) const;
		void blit(const glm::ivec4 &, const glm::ivec4 &, GLbitfield, GLenum) const;
		void blitAttachment(GLenum, const glm::ivec4 &) const;

	private:

		GLObject m_obj;
		mutable GLenum m_target;

};

} // namespace gl
