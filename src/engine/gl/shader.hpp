#pragma once

#include "globject.hpp"

#include <string>

namespace gl {

class Shader {

	public:

		Shader(GLenum);
		Shader(const std::string &);
		Shader(const Shader &) = delete;
		Shader(Shader &&) = default;
		Shader & operator=(const Shader &) = delete;
		Shader & operator=(Shader &&) = default;
		~Shader() {}

		operator GLuint() const { return m_obj; }

		void addSourceFromString(const std::string &);
		void addSourceFromFile(const std::string &);
		bool compileSource() const;

	private:

		auto getCode(const std::string &) const;
		bool allocate(const std::string &);
		bool compile(const std::string &);

		GLObject m_obj;

		std::string m_source;

};

} // namespace gl
