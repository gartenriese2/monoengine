#include <cstdlib>

//#include "demo.hpp"
#include "engine/engine.hpp"
#include "engine/gl/buffer.hpp"
#include "engine/extern/imgui.h"

int main() {

	//Demo demo({800, 600});
	//while (demo.render());

	engine::Engine e({800, 600}, "Test", true);

	e.getInputPtr()->addKeyFunc([&](const int key, const int, const int action, const int){
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(e.getWindowPtr()->getGLFWWindow(), GL_TRUE);
		}
	});

	glClearColor(0,0,0,0);

	while (e.render()) {
		glClear(GL_COLOR_BUFFER_BIT);
		e.getGuiPtr()->update();
		ImGui::Text("Hallo Welt");
		e.getGuiPtr()->render();
	}

	return EXIT_SUCCESS;

}
