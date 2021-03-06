#include "globject.hpp"

#include "../core/log.hpp"

namespace gl {

GLObject::GLObject()
  : m_handle{},
	m_deleter{[](GLuint &){}}
{

}

GLObject::GLObject(const GLenum type)
  : m_handle{},
	m_deleter{[](GLuint &){}}
{

	switch (type) {
		case GL_VERTEX_SHADER:
		case GL_FRAGMENT_SHADER:
		case GL_GEOMETRY_SHADER:
		case GL_TESS_CONTROL_SHADER:
		case GL_COMPUTE_SHADER:
		case GL_TESS_EVALUATION_SHADER:
			m_handle = glCreateShader(type);
			m_deleter = [](GLuint & handle){ glDeleteShader(handle); };
			break;
		case GL_PROGRAM:
			m_handle = glCreateProgram();
			m_deleter = [](GLuint & handle){ glDeleteProgram(handle); };
			break;
		case GL_BUFFER:
			glCreateBuffers(1, &m_handle);
			m_deleter = [](GLuint & handle){ glDeleteBuffers(1, &handle); };
			break;
		case GL_VERTEX_ARRAY:
			glCreateVertexArrays(1, &m_handle);
			m_deleter = [](GLuint & handle){ glDeleteVertexArrays(1, &handle); };
			break;
		case GL_TEXTURE_2D:
			glCreateTextures(type, 1, &m_handle);
			m_deleter = [](GLuint & handle){ glDeleteTextures(1, &handle); };
			break;
		case GL_TIME_ELAPSED:
			glCreateQueries(type, 1, &m_handle);
			m_deleter = [](GLuint & handle){ glDeleteQueries(1, &handle); };
			break;
		case GL_FRAMEBUFFER:
			glCreateFramebuffers(1, &m_handle);
			m_deleter = [](GLuint & handle){ glDeleteFramebuffers(1, &handle); };
			break;
		default:
			LOG("Unimplemented GLenum!");
	}

}

GLObject::GLObject(GLObject && other) noexcept
  : m_handle{std::move(other.m_handle)},
	m_deleter{std::move(other.m_deleter)}
{
	other.m_handle = 0;
	other.m_deleter = [](GLuint &){};
}

GLObject & GLObject::operator=(GLObject && other) & noexcept {
	std::swap(m_handle, other.m_handle);
	std::swap(m_deleter, other.m_deleter);
	return *this;
}

GLObject::~GLObject() {
	if (m_deleter) {
		m_deleter(m_handle);
	}
}

} // namespace gl
