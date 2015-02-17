#pragma once

#include "../glfwinclude.hpp"

#include <vector>
#include <functional>

namespace core {

class Input {

	public:

		Input(GLFWwindow * const);

	private:

		GLFWwindow * const m_window;

		std::vector<std::function<void(int, int, int, int)>> m_keyFuncs;
		std::vector<std::function<void(double, double)>> m_mouseCursorFuncs;
		std::vector<std::function<void(int, int, int)>> m_mouseButtonFuncs;
		std::vector<std::function<void(double, double)>> m_mouseScrollFuncs;

};

} // namespace core
