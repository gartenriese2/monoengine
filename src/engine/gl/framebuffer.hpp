#pragma once

#include "globject.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace gl {

class Framebuffer {

	public:

		Framebuffer();
		Framebuffer(const std::string & name);
		Framebuffer(const Framebuffer &) = delete;
		Framebuffer(Framebuffer &&) = default;
		Framebuffer & operator=(const Framebuffer &) = delete;
		Framebuffer & operator=(Framebuffer &&) & = default;
		~Framebuffer() {}

		operator GLuint() const noexcept { return m_obj; }

		void bind(GLenum target = GL_FRAMEBUFFER) const;
		void unbind(GLenum target = GL_FRAMEBUFFER) const;

		GLint getParameter(GLenum pname) const;
		void setParameter(GLenum pname, GLint param) const;
		GLint getAttachmentParameter(GLenum attachment, GLenum pname) const;
		const glm::ivec2 getAttachmentSize(GLenum attachment) const;

		void attachTexture(GLenum attachment, const gl::Texture & texture, GLint level) const;

		bool isComplete(GLenum target) const;

		void read(GLenum src) const;
		void draw(const std::vector<GLenum> & buffers) const;
		void blit(GLuint drawFramebuffer, const glm::ivec4 & src, const glm::ivec4 & dst, GLbitfield mask, GLenum filter) const;
		void blitAttachment(GLenum src, const glm::ivec4 & dst) const;

	private:

		GLObject m_obj;

};

static_assert(std::is_nothrow_move_constructible<Framebuffer>(), "Should be noexcept MoveConstructible");
static_assert(!std::is_copy_constructible<Framebuffer>(), "Should not be CopyConstructible");
static_assert(std::is_nothrow_move_assignable<Framebuffer>(), "Should be noexcept MoveAssignable");
static_assert(!std::is_copy_assignable<Framebuffer>(), "Should not be CopyAssignable");

} // namespace gl
