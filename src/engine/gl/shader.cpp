#include "shader.hpp"

#include "../core/log.hpp"

#include <fstream>
#include <vector>

namespace gl {

Shader::Shader(const GLenum type)
  : m_obj{type}
{

}

Shader::Shader(const std::string & file) {
	if (!allocate(file)) return;
	if (!compile(file)) return;
}

Shader::Shader(const std::string & file, const std::string & name) {
	if (!allocate(file)) return;
	if (!compile(file)) return;
	glObjectLabel(GL_SHADER, m_obj, static_cast<GLsizei>(name.size()), name.c_str());
}

auto Shader::getCode(const std::string & file) const {

	std::string code;
	std::ifstream stream(file, std::ifstream::in);
	if (!stream.is_open()) {
		LOG("Failed to open file: " + file);
		return code;
	}
	while (stream.good()) {
		std::string tmp;
		std::getline(stream, tmp);
		code += tmp + "\n";
	}
	stream.close();
	return code;

}

void Shader::addSourceFromString(const std::string & str) {
	m_source.append(str);
}

void Shader::addSourceFromFile(const std::string & file) {
	m_source.append(getCode(file));
}

bool Shader::compileSource() const {

	const auto charcode = m_source.c_str();
	const auto len = static_cast<GLint>(m_source.size());

	glShaderSource(m_obj, 1, &charcode, &len);
	glCompileShader(m_obj);

	auto success = GL_FALSE;
	glGetShaderiv(m_obj, GL_COMPILE_STATUS, &success);

	if (!success) {

		auto logSize = GLint{};
		glGetShaderiv(m_obj, GL_INFO_LOG_LENGTH, &logSize);

		std::vector<char> tmp;
		tmp.reserve(static_cast<size_t>(logSize));
		glGetShaderInfoLog(m_obj, logSize, NULL, &tmp[0]);

		if (tmp.size() > 0) {
			LOG("Shader compiling failed with the following error:\n", &tmp[0]);
		} else {
			LOG("Shader compiling failed.");
		}
		return false;

	}

	return true;

}

bool Shader::allocate(const std::string & file) {
	DEB
	if (file.size() < 6 && file[file.size() - 5] != '.') {
		LOG(file + " is not a valid shader name!");
		return false;
	}
	DEB
	const auto type = file.substr(file.size() - 4);
	if (type == "vert") {
		auto obj = GLObject(GL_VERTEX_SHADER);
		DEB
		std::swap(m_obj, obj);
		DEB
	} else if (type == "frag") {
		auto obj = GLObject(GL_FRAGMENT_SHADER);
		std::swap(m_obj, obj);
	} else if (type == "geom") {
		auto obj = GLObject(GL_GEOMETRY_SHADER);
		std::swap(m_obj, obj);
	} else if (type == "cont") {
		auto obj = GLObject(GL_TESS_CONTROL_SHADER);
		std::swap(m_obj, obj);
	} else if (type == "eval") {
		auto obj = GLObject(GL_TESS_EVALUATION_SHADER);
		std::swap(m_obj, obj);
	} else if (type == "comp") {
		auto obj = GLObject(GL_COMPUTE_SHADER);
		std::swap(m_obj, obj);
	} else {
		LOG("Not a valid shader file ending: " + type);
		return false;
	}
	DEB
	if (m_obj == 0) {
		LOG("Error allocating shader object: " + file);
		return false;
	}
	DEB
	return true;

}

bool Shader::compile(const std::string & file) {

	const auto code = getCode(file);
	const auto charcode = code.c_str();
	const auto len = static_cast<GLint>(code.size());

	glShaderSource(m_obj, 1, &charcode, &len);
	glCompileShader(m_obj);

	auto success = GL_FALSE;
	glGetShaderiv(m_obj, GL_COMPILE_STATUS, &success);

	if (!success) {

		auto logSize = GLint{};
		glGetShaderiv(m_obj, GL_INFO_LOG_LENGTH, &logSize);

		std::vector<char> tmp;
		tmp.reserve(static_cast<size_t>(logSize));
		glGetShaderInfoLog(m_obj, logSize, NULL, &tmp[0]);

		if (tmp.size() > 0) {
			LOG(file + ": Shader compiling failed with the following error:\n", &tmp[0]);
		} else {
			LOG(file + ": Shader compiling failed.");
		}
		return false;

	}

	return true;

}

} // namespace gl
