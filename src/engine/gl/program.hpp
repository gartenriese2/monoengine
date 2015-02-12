#pragma once

#include "uniform.hpp"
#include "globject.hpp"

#include <map>

namespace gl {

class Shader;

class Program {

 	public:

		Program();
		Program(const std::string &);
		Program(const Program &) = delete;
		Program(Program &&) = default;
		Program & operator=(const Program &) = delete;
		Program & operator=(Program &&) = default;
		~Program() {}

 		operator GLuint() const { return m_obj; }
 		auto operator[](const std::string &) const;

		void attachShader(const Shader &) const;
		void use();

 	private:

		bool link();

		GLObject m_obj;
		std::map<const std::string, GLint> m_uniforms;

};

} // namespace gl
