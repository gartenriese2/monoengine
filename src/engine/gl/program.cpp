#include "program.hpp"

#include "shader.hpp"
#include "../core/log.hpp"

#include <vector>
#include <memory>

namespace gl {

Program::Program()
  : m_obj{GL_PROGRAM}
{
	if (m_obj == 0) {
		LOG("Error creating Program!");
	}
}

Program::Program(const std::string & name)
  : m_obj{GL_PROGRAM}
{
	if (m_obj == 0) {
		LOG("Error creating Program " + name);
	} else {
		glObjectLabel(GL_PROGRAM, m_obj, static_cast<GLsizei>(name.size()), name.c_str());
	}
}

Uniform Program::operator[](const std::string & name) const {

	auto it {m_uniforms.find(name)};
	if (it != m_uniforms.end()) {
		return Uniform(it->second);
	}
	return Uniform(-1);

}


void Program::attachShader(const Shader & shader) const {
	glAttachShader(m_obj, shader);
}

bool Program::link() {

	glLinkProgram(m_obj);

	auto success {GL_FALSE};
	glGetProgramiv(m_obj, GL_LINK_STATUS, &success);

	if (!success) {

		auto logSize {0};
		glGetProgramiv(m_obj, GL_INFO_LOG_LENGTH, &logSize);

		std::string log(static_cast<size_t>(logSize), '\0');

		GLsizei len;
		glGetProgramInfoLog(m_obj, logSize, &len, &log[0]);

		LOG("Program linking failed with the following error:\n");
		if (len > 0) LOG(log);

		return false;

	}

	m_uniforms.clear();

	auto numUniforms {0};
	glGetProgramiv(m_obj, GL_ACTIVE_UNIFORMS, &numUniforms);
	auto maxLength {0};
	glGetProgramiv(m_obj, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
	std::unique_ptr<char[]> buffer{new char[static_cast<unsigned long>(maxLength)]};
	auto size {1};
	auto length {1};
	auto type = GLenum{};
	for (auto i {0u}; i < static_cast<GLuint>(numUniforms); ++i) {

		glGetActiveUniform(m_obj, i, maxLength, &length, &size, &type, buffer.get());
		const auto loc {glGetUniformLocation(m_obj, buffer.get())};
		if (loc != -1) m_uniforms.emplace(std::string(buffer.get()), loc);

	}

	return true;

}

void Program::use() {

	auto linkStatus {0};
	glGetProgramiv(m_obj, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == 0) {
		link();
	}

	glUseProgram(m_obj);

}

} // namespace gl
