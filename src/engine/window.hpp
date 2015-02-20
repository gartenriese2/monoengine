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
		unsigned int getWidth() const { return m_width; }
		unsigned int getHeight() const { return m_height; }

		bool render();

	private:

		GLFWwindow * m_win;

		unsigned int m_width;
		unsigned int m_height;

};

} // namespace engine
