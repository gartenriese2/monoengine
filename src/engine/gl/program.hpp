#pragma once

#include "uniform.hpp"
#include "globject.hpp"

#include <map>

namespace gl {

class Shader;

class Program {

 	public:

		Program();
		Program(const std::string & name);
		Program(const Program &) = delete;
		Program(Program &&) = default;
		Program & operator=(const Program &) = delete;
		Program & operator=(Program &&) & = default;
		~Program() {}

 		// explicit operator GLuint() const { return m_obj; }
 		Uniform operator[](const std::string & name) const;

		void attachShader(const Shader & shader) const;
		void use();

 	private:

		bool link();

		GLObject m_obj;
		std::map<const std::string, GLint> m_uniforms;

};

static_assert(std::is_nothrow_move_constructible<Program>(), "Should be noexcept MoveConstructible");
static_assert(!std::is_copy_constructible<Program>(), "Should not be CopyConstructible");
static_assert(std::is_nothrow_move_assignable<Program>(), "Should be noexcept MoveAssignable");
static_assert(!std::is_copy_assignable<Program>(), "Should not be CopyAssignable");

} // namespace gl
