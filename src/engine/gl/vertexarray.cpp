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

VertexArray::VertexArray(const std::string & name)
  : m_obj{GL_VERTEX_ARRAY}
{
	if (m_obj == 0) {
		LOG("Error creating VertexArray " + name);
	} else {
		glObjectLabel(GL_VERTEX_ARRAY, m_obj, static_cast<GLsizei>(name.size()), name.c_str());
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
	glEnableVertexArrayAttrib(m_obj, bindingIndex);
}

void VertexArray::bindElementBuffer(const GLuint buffer) const {
	glVertexArrayElementBuffer(m_obj, buffer);
}

void VertexArray::bindVertexBuffer(const unsigned int bindingIndex, const GLuint buffer,
		const unsigned int offset, const unsigned int stride, const unsigned int divisor) const {
	glVertexArrayVertexBuffer(m_obj, bindingIndex, buffer, offset, static_cast<GLsizei>(stride));
	glVertexArrayBindingDivisor(m_obj, bindingIndex, divisor);
}

void VertexArray::bindVertexFormat(const unsigned int bindingIndex, const unsigned int attribIndex,
		const unsigned int size, const GLenum type, const bool normalized,
		const unsigned int relativeOffset) const {
	if (!attribBindingEnabled(attribIndex)) {
		LOG_WARNING("Binding index " + std::to_string(attribIndex) + " is not enabled!");
		return;
	}
	glVertexArrayAttribFormat(m_obj, attribIndex, static_cast<GLint>(size), type, normalized, relativeOffset);
	glVertexArrayAttribBinding(m_obj, attribIndex, bindingIndex);
}

} // namespace gl
