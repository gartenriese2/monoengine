#include "buffer.hpp"

#include "../core/log.hpp"

namespace gl {

Buffer::Buffer()
  : m_obj{GL_BUFFER}
{
	if (m_obj == 0) {
		LOG("Error creating Buffer!");
	}
}

Buffer::Buffer(const std::string & name)
  : m_obj{GL_BUFFER}
{
	if (m_obj == 0) {
		LOG("Error creating Buffer " + name);
	} else {
		glObjectLabel(GL_BUFFER, m_obj, static_cast<GLsizei>(name.size()), name.c_str());
	}
}

void Buffer::bind(const GLenum target) const {
	glBindBuffer(target, m_obj);
}

void Buffer::unbind(const GLenum target) const {
	glBindBuffer(target, 0);
}

unsigned int Buffer::getSize(const GLenum target) const {
	GLint ret;
	glGetBufferParameteriv(target, GL_BUFFER_SIZE, &ret);
	return static_cast<unsigned int>(ret);
}

void Buffer::createMutableStorage(const GLenum target, const unsigned int size, const GLbitfield usage, const void * data) {
	glBufferData(target, size, data, usage);
}

void Buffer::setData(const GLenum target, const unsigned int offset, const unsigned int size, const void * data) {
	glBufferSubData(target, offset, size, data);
}

bool Buffer::isMapped(const GLenum target) const {
	GLint ret;
	glGetBufferParameteriv(target, GL_BUFFER_MAPPED, &ret);
	return static_cast<bool>(ret);
}

void * Buffer::map(const GLenum target, const unsigned int offset, const unsigned int size, const GLbitfield access) {
	return glMapBufferRange(target, offset, size, access);
}

bool Buffer::unmap(const GLenum target) const {
	return glUnmapBuffer(target);
}

} // namespace gl
