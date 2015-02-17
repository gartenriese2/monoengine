#pragma once

#include "glinclude.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gl {

class Uniform {

	public:

		/*
		 *	single values
		 */

		// bool
		void operator=(const bool val) {
			if (m_loc != -1) {
				glUniform1i(m_loc, val);
			}
		}

		// int
		void operator=(const int val) {
			if (m_loc != -1) {
				glUniform1i(m_loc, val);
			}
		}

		// unsigned int
		void operator=(const unsigned int val) {
			if (m_loc != -1) {
				glUniform1ui(m_loc, val);
			}
		}

		// float
		void operator=(const float val) {
			if (m_loc != -1) {
				glUniform1f(m_loc, val);
			}
		}

		// double
		void operator=(const double val) {
			if (m_loc != -1) {
				glUniform1d(m_loc, val);
			}
		}

		/*
		 *	vectors
		 */

		// bool
		void operator=(const glm::bvec2 & vec) {
			if (m_loc != -1) {
				glUniform2i(m_loc, vec.x, vec.y);
			}
		}
		void operator=(const glm::bvec3 & vec) {
			if (m_loc != -1) {
				glUniform3i(m_loc, vec.x, vec.y, vec.z);
			}
		}
		void operator=(const glm::bvec4 & vec) {
			if (m_loc != -1) {
				glUniform4i(m_loc, vec.x, vec.y, vec.z, vec.w);
			}
		}

		// int
		void operator=(const glm::ivec2 & vec) {
			if (m_loc != -1) {
				glUniform2iv(m_loc, 1, glm::value_ptr(vec));
			}
		}
		void operator=(const glm::ivec3 & vec) {
			if (m_loc != -1) {
				glUniform3iv(m_loc, 1, glm::value_ptr(vec));
			}
		}
		void operator=(const glm::ivec4 & vec) {
			if (m_loc != -1) {
				glUniform4iv(m_loc, 1, glm::value_ptr(vec));
			}
		}

		// unsigned int
		void operator=(const glm::uvec2 & vec) {
			if (m_loc != -1) {
				glUniform2uiv(m_loc, 1, glm::value_ptr(vec));
			}
		}
		void operator=(const glm::uvec3 & vec) {
			if (m_loc != -1) {
				glUniform3uiv(m_loc, 1, glm::value_ptr(vec));
			}
		}
		void operator=(const glm::uvec4 & vec) {
			if (m_loc != -1) {
				glUniform4uiv(m_loc, 1, glm::value_ptr(vec));
			}
		}

		// float
		void operator=(const glm::vec2 & vec) {
			if (m_loc != -1) {
				glUniform2fv(m_loc, 1, glm::value_ptr(vec));
			}
		}
		void operator=(const glm::vec3 & vec) {
			if (m_loc != -1) {
				glUniform3fv(m_loc, 1, glm::value_ptr(vec));
			}
		}
		void operator=(const glm::vec4 & vec) {
			if (m_loc != -1) {
				glUniform4fv(m_loc, 1, glm::value_ptr(vec));
			}
		}

		// double
		void operator=(const glm::dvec2 & vec) {
			if (m_loc != -1) {
				glUniform2dv(m_loc, 1, glm::value_ptr(vec));
			}
		}
		void operator=(const glm::dvec3 & vec) {
			if (m_loc != -1) {
				glUniform3dv(m_loc, 1, glm::value_ptr(vec));
			}
		}
		void operator=(const glm::dvec4 & vec) {
			if (m_loc != -1) {
				glUniform4dv(m_loc, 1, glm::value_ptr(vec));
			}
		}

		/*
		 *	matrices
		 */

		// float
		void operator=(const glm::mat2 & mat) {
			if (m_loc != -1) {
				glUniformMatrix2fv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::mat3 & mat) {
			if (m_loc != -1) {
				glUniformMatrix3fv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::mat4 & mat) {
			if (m_loc != -1) {
				glUniformMatrix4fv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::mat2x3 & mat) {
			if (m_loc != -1) {
				glUniformMatrix2x3fv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::mat3x2 & mat) {
			if (m_loc != -1) {
				glUniformMatrix3x2fv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::mat2x4 & mat) {
			if (m_loc != -1) {
				glUniformMatrix2x4fv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::mat4x2 & mat) {
			if (m_loc != -1) {
				glUniformMatrix4x2fv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::mat3x4 & mat) {
			if (m_loc != -1) {
				glUniformMatrix3x4fv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::mat4x3 & mat) {
			if (m_loc != -1) {
				glUniformMatrix4x3fv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}

		// double
		void operator=(const glm::dmat2 & mat) {
			if (m_loc != -1) {
				glUniformMatrix2dv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::dmat3 & mat) {
			if (m_loc != -1) {
				glUniformMatrix3dv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::dmat4 & mat) {
			if (m_loc != -1) {
				glUniformMatrix4dv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::dmat2x3 & mat) {
			if (m_loc != -1) {
				glUniformMatrix2x3dv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::dmat3x2 & mat) {
			if (m_loc != -1) {
				glUniformMatrix3x2dv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::dmat2x4 & mat) {
			if (m_loc != -1) {
				glUniformMatrix2x4dv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::dmat4x2 & mat) {
			if (m_loc != -1) {
				glUniformMatrix4x2dv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::dmat3x4 & mat) {
			if (m_loc != -1) {
				glUniformMatrix3x4dv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}
		void operator=(const glm::dmat4x3 & mat) {
			if (m_loc != -1) {
				glUniformMatrix4x3dv(m_loc, 1, GL_FALSE, glm::value_ptr(mat));
			}
		}

	private:

		friend class Program;
		explicit Uniform(GLint loc) : m_loc{loc} {}

		GLint m_loc;

};

} // namespace gl
