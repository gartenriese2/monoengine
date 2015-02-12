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

void VertexArray::bindElementBuffer(const GLuint buffer) const {
	glVertexArrayElementBuffer(m_obj, buffer);
}

void VertexArray::bindVertexBuffer(const GLuint buffer, const unsigned int bindingIndex,
		const unsigned int offset, const unsigned int stride) const {
	glEnableVertexArrayAttrib(m_obj, bindingIndex);
	glVertexArrayVertexBuffer(m_obj, bindingIndex, buffer, offset, static_cast<GLsizei>(stride));
}

} // namespace gl
