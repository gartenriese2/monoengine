#include <cstdlib>

#include "demo.hpp"
#include "engine\extern\imgui.h"

int main() {

	//Demo demo({1920, 1080});
	//while (demo.render());

	engine::Engine e({ 1920, 1080 }, "Test");

	GLuint handle2 = 0;
	auto handle = glCreateShader(GL_VERTEX_SHADER);
	std::swap(handle, handle2);

	while (e.render()) {
		glClear(GL_COLOR_BUFFER_BIT);
		e.getGuiPtr()->update();
		ImGui::Text("Hallo Welt");
		e.getGuiPtr()->render();
	}

	return EXIT_SUCCESS;

}
