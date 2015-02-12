#pragma once

#include "globject.hpp"
#include "../debug.hpp"
#include <string>

namespace gl {

class Shader {

	public:

		Shader(const std::string &);
		Shader(const std::string &, const std::string &);
		Shader(const Shader &) = delete;
		Shader(Shader &&) = default;
		Shader & operator=(const Shader &) = delete;
		Shader & operator=(Shader &&) = default;
		~Shader() {}

		operator GLuint() const { return m_obj; }

	private:

		auto getCode(const std::string &) const;
		bool allocate(const std::string &);
		bool compile(const std::string &);

		GLObject m_obj;

};

} // namespace gl
