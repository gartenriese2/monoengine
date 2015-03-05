#include "buffer.hpp"

#include "../core/log.hpp"

namespace gl {

Buffer::Buffer()
  : m_obj{GL_BUFFER},
	m_target{GL_NONE}
{
	if (m_obj == 0) {
		LOG("Error creating Buffer!");
	}
}

void Buffer::bind(const GLenum target) const {
	glBindBuffer(target, m_obj);
	m_target = target;
}

void Buffer::unbind() const {
	glBindBuffer(m_target, 0);
	m_target = GL_NONE;
}

unsigned int Buffer::getSize() const {
	GLint ret;
	glGetBufferParameteriv(m_target, GL_BUFFER_SIZE, &ret);
	return static_cast<unsigned int>(ret);
}

void Buffer::createMutableStorage(const unsigned int size, const GLbitfield usage, const void * data) {
	glBufferData(m_target, size, data, usage);
}

void Buffer::setData(const unsigned int offset, const unsigned int size, const void * data) {
	glBufferSubData(m_target, offset, size, data);
}

bool Buffer::isMapped() const {
	GLint ret;
	glGetBufferParameteriv(m_target, GL_BUFFER_MAPPED, &ret);
	return static_cast<bool>(ret);
}

void * Buffer::map(const unsigned int offset, const unsigned int size, const GLbitfield access) {
	return glMapBufferRange(m_target, offset, size, access);
}

bool Buffer::unmap() const {
	return glUnmapBuffer(m_target);
}

} // namespace gl
