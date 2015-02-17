#pragma once

#include "extern/glfwinclude.hpp"

#include <string>

namespace engine {

class Window {

	public:

		Window() : m_win{nullptr} {}
		Window(unsigned int, unsigned int, const std::string &);
		~Window();

		GLFWwindow * getGLFWWindow() { return m_win; }

		bool render();

	private:

		GLFWwindow * m_win;

};

} // namespace engine
