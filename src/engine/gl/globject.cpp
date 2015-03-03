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
			glGenBuffers(1, &m_handle);
			m_deleter = [](GLuint & handle){ glDeleteBuffers(1, &handle); };
			break;
		case GL_VERTEX_ARRAY:
			glGenVertexArrays(1, &m_handle);
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
			glGenTextures(1, &m_handle);
			m_deleter = [](GLuint & handle){ glDeleteTextures(1, &handle); };
			break;
		case GL_TIME_ELAPSED:
			glGenQueries(1, &m_handle);
			m_deleter = [](GLuint & handle){ glDeleteQueries(1, &handle); };
			break;
		case GL_FRAMEBUFFER:
			glGenFramebuffers(1, &m_handle);
			m_deleter = [](GLuint & handle){ glDeleteFramebuffers(1, &handle); };
			break;
		default:
			LOG("Unimplemented GLenum!");
	}

}

GLObject::~GLObject() {
	m_deleter(m_handle);
}

} // namespace gl
