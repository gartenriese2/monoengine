#pragma once

#include "../extern/glfwinclude.hpp"

#include <vector>
#include <functional>

namespace core {

class Input {

	public:

		Input(GLFWwindow * const);

		void addKeyFunc(const std::function<void(int, int, int, int)> &);
		void addCharFunc(const std::function<void(unsigned int)> &);
		void addMouseCursorFunc(const std::function<void(double, double)> &);
		void addMouseButtonFunc(const std::function<void(int, int, int)> &);
		void addMouseScrollFunc(const std::function<void(double, double)> &);

	private:

		GLFWwindow * const m_window;

		std::vector<std::function<void(int, int, int, int)>> m_keyFuncs;
		std::vector<std::function<void(unsigned int)>> m_charFuncs;
		std::vector<std::function<void(double, double)>> m_mouseCursorFuncs;
		std::vector<std::function<void(int, int, int)>> m_mouseButtonFuncs;
		std::vector<std::function<void(double, double)>> m_mouseScrollFuncs;

};

} // namespace core
