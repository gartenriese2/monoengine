#include "globject.hpp"

#include "../debug.hpp"

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
		case GL_TEXTURE_1D:
		case GL_TEXTURE_2D:
		case GL_TEXTURE_3D:
		case GL_TEXTURE_1D_ARRAY:
		case GL_TEXTURE_2D_ARRAY:
		case GL_TEXTURE_RECTANGLE:
		case GL_TEXTURE_CUBE_MAP:
		case GL_TEXTURE_CUBE_MAP_ARRAY:
		case GL_TEXTURE_BUFFER:
		case GL_TEXTURE_2D_MULTISAMPLE:
		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
			glCreateTextures(type, 1, &m_handle);
			m_deleter = [](GLuint & handle){ glDeleteTextures(1, &handle); };
			break;
		case GL_TIME_ELAPSED:
			glCreateQueries(type, 1, &m_handle);
			m_deleter = [](GLuint & handle){ glDeleteQueries(1, &handle); };
			break;
		default:
			LOG("Unimplemented GLenum!");
	}

}

GLObject::~GLObject() {
	m_deleter(m_handle);
}

} // namespace gl
