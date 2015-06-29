#pragma once

#include "globject.hpp"

#include <string>

namespace gl {

class Shader {

	public:

		Shader(GLenum type);
		Shader(const std::string & file);
		Shader(const std::string & file, const std::string & name);
		Shader(const Shader &) = delete;
		Shader(Shader &&) = default;
		Shader & operator=(const Shader &) = delete;
		Shader & operator=(Shader && other) & noexcept;
		~Shader() {}

		operator GLuint() const noexcept { return m_obj; }

		void addSourceFromString(const std::string & str);
		void addSourceFromFile(const std::string & file);
		bool compileSource() const;

	private:

		auto getCode(const std::string & file) const;
		bool allocate(const std::string & file);
		bool compile(const std::string & file);

		GLObject m_obj;

		std::string m_source;

};

static_assert(std::is_nothrow_move_constructible<Shader>(), "Should be noexcept MoveConstructible");
static_assert(!std::is_copy_constructible<Shader>(), "Should not be CopyConstructible");
static_assert(std::is_nothrow_move_assignable<Shader>(), "Should be noexcept MoveAssignable");
static_assert(!std::is_copy_assignable<Shader>(), "Should not be CopyAssignable");

} // namespace gl
