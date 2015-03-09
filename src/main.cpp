#include <cstdlib>

//#include "demo.hpp"
//#include "engine/extern/imgui.h"
//#include "engine/gl/buffer.hpp"
//#include "engine/gl/program.hpp"
//#include "engine/gl/shader.hpp"
//#include "engine/core/log.hpp"
//#include "engine/gl/globject.hpp"
//
//#include <fstream>
//#include <iostream>
#include <functional>

class Foo {
public:
	Foo(int) : m_deleter{ []() {} }	{}
	Foo(const Foo &) = delete;
	Foo(Foo &&) = default;
	Foo & operator=(const Foo &) = default;
	Foo & operator=(Foo &&) = default;
	~Foo()
	{
		m_deleter();
	}
private:
	std::function<void()> m_deleter;
};

int main() {

	// Demo demo({1920, 1080});
	// while (demo.render());

	Foo foo(1);
	Foo bar(2);
	std::swap(foo, bar);

	//engine::Engine e({ 1920, 1080 }, "Test");


	//gl::Buffer vbo("test vbo");
	//gl::Shader vert("shader/test/instancedraw.vert", "instance_vert");

	/*gl::GLObject m_obj;
	
	const std::string file{ "../shader/test/instancedraw.vert" };

	if (file.size() < 6 && file[file.size() - 5] != '.') {
		LOG(file + " is not a valid shader name!");
		system("PAUSE");
		return false;
	}
	gl::GLObject obj = gl::GLObject(GL_VERTEX_SHADER);*/
	//std::swap(m_obj, obj);
	/*const auto type = file.substr(file.size() - 4);
	if (type == "vert") {
		auto obj = gl::GLObject(GL_VERTEX_SHADER);
		std::swap(m_obj, obj);
	}
	else if (type == "frag") {
		auto obj = gl::GLObject(GL_FRAGMENT_SHADER);
		std::swap(m_obj, obj);
	}
	else if (type == "geom") {
		auto obj = gl::GLObject(GL_GEOMETRY_SHADER);
		std::swap(m_obj, obj);
	}
	else if (type == "cont") {
		auto obj = gl::GLObject(GL_TESS_CONTROL_SHADER);
		std::swap(m_obj, obj);
	}
	else if (type == "eval") {
		auto obj = gl::GLObject(GL_TESS_EVALUATION_SHADER);
		std::swap(m_obj, obj);
	}
	else if (type == "comp") {
		auto obj = gl::GLObject(GL_COMPUTE_SHADER);
		std::swap(m_obj, obj);
	}
	else {
		LOG("Not a valid shader file ending: " + type);
		system("PAUSE");
		return false;
	}*/

	/*std::string code;
	std::ifstream stream(file, std::ifstream::in);
	if (!stream.is_open()) {
		LOG("Failed to open file: " + file);
		system("PAUSE");
		return EXIT_FAILURE;
	}
	while (stream.good()) {
		std::string tmp;
		std::getline(stream, tmp);
		code += tmp + "\n";
	}
	stream.close();
	LOG(code);

	while (e.render()) {
		glClear(GL_COLOR_BUFFER_BIT);
		e.getGuiPtr()->update();
		ImGui::Text("Hallo Welt");
		e.getGuiPtr()->render();
	}*/

	return EXIT_SUCCESS;

}
