#include "vertexarray.hpp"

#include "../debug.hpp"


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

bool VertexArray::attribBindingEnabled(const unsigned int bindingIndex) const {
	bind();
	GLint ret;
	glGetVertexAttribiv(bindingIndex, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &ret);
	return static_cast<bool>(ret);
}

void VertexArray::enableAttribBinding(const unsigned int bindingIndex) const {
	glEnableVertexArrayAttrib(m_obj, bindingIndex);
}

void VertexArray::bindElementBuffer(const GLuint buffer) const {
	glVertexArrayElementBuffer(m_obj, buffer);
}

void VertexArray::bindVertexBuffer(const GLuint buffer, const unsigned int bindingIndex,
		const unsigned int offset, const unsigned int stride, const unsigned int divisor) const {
	if (!attribBindingEnabled(bindingIndex)) {
		LOG_WARNING("Binding index " + std::to_string(bindingIndex) + " is not enabled!");
		return;
	}
	glVertexArrayVertexBuffer(m_obj, bindingIndex, buffer, offset, static_cast<GLsizei>(stride));
	glVertexArrayBindingDivisor(m_obj, bindingIndex, divisor);
}

void VertexArray::bindVertexFormat(const unsigned int bindingIndex, const unsigned int size,
		const GLenum type, const bool normalized, const unsigned int offset) const {
	if (!attribBindingEnabled(bindingIndex)) {
		LOG_WARNING("Binding index " + std::to_string(bindingIndex) + " is not enabled!");
		return;
	}
	glVertexAttribFormat(bindingIndex, static_cast<GLint>(size), type, normalized, offset);
	glVertexAttribBinding(bindingIndex, bindingIndex);
}

} // namespace gl
