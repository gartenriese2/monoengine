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

unsigned int Buffer::getSize() const {
	GLint ret;
	glGetNamedBufferParameteriv(m_obj, GL_BUFFER_SIZE, &ret);
	return static_cast<unsigned int>(ret);
}

void Buffer::createImmutableStorage(const unsigned int size, const GLbitfield flags, const void * data) {
	glNamedBufferStorage(m_obj, size, data, flags);
}

void Buffer::createMutableStorage(const unsigned int size, const GLbitfield usage, const void * data) {
	glNamedBufferData(m_obj, size, data, usage);
}

void Buffer::setData(const unsigned int offset, const unsigned int size, const void * data) {
	glNamedBufferSubData(m_obj, offset, size, data);
}

bool Buffer::isMapped() const {
	GLint ret;
	glGetNamedBufferParameteriv(m_obj, GL_BUFFER_MAPPED, &ret);
	return static_cast<bool>(ret);
}

void * Buffer::map(const unsigned int offset, const unsigned int size, const GLbitfield access) {
	return glMapNamedBufferRange(m_obj, offset, size, access);
}

bool Buffer::unmap() const {
	return glUnmapNamedBuffer(m_obj);
}

} // namespace gl
