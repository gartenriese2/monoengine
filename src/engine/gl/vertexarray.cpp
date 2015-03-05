#include "vertexarray.hpp"

#include "../core/log.hpp"


namespace gl {

VertexArray::VertexArray()
  : m_obj{GL_VERTEX_ARRAY}
{
	if (m_obj == 0) {
		LOG("Error creating VertexArray!");
	}
}

void VertexArray::bind() const {
	glBindVertexArray(m_obj);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

bool VertexArray::attribBindingEnabled(const unsigned int attribIndex) const {
	bind();
	GLint ret;
	glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &ret);
	return static_cast<bool>(ret);
}

void VertexArray::enableAttribBinding(const unsigned int bindingIndex) const {
	glEnableVertexAttribArray(bindingIndex);
}

void VertexArray::bindVertexBuffer(const unsigned int bindingIndex, const GLuint buffer,
		const unsigned int offset, const unsigned int stride, const unsigned int divisor) const {
	glBindVertexBuffer(bindingIndex, buffer, offset, static_cast<GLsizei>(stride));
	glVertexBindingDivisor(bindingIndex, divisor);
}

void VertexArray::bindVertexFormat(const unsigned int bindingIndex, const unsigned int attribIndex,
		const unsigned int size, const GLenum type, const bool normalized,
		const unsigned int relativeOffset) const {
	if (!attribBindingEnabled(attribIndex)) {
		LOG_WARNING("Binding index " + std::to_string(attribIndex) + " is not enabled!");
		return;
	}
	glVertexAttribFormat(attribIndex, static_cast<GLint>(size), type, normalized, relativeOffset);
	glVertexAttribBinding(attribIndex, bindingIndex);
}

} // namespace gl
