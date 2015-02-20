#include "window.hpp"

#include "core/log.hpp"

namespace engine {

Window::Window(const unsigned int width, const unsigned int height, const std::string & name)
  : m_win{glfwCreateWindow(static_cast<int>(width), static_cast<int>(height),
			name.c_str(), NULL, NULL)},
	m_width{width},
	m_height{height}
{
	if (!m_win) {
		LOG_ERROR("Could not create GLFW Window!");
	}
	glfwMakeContextCurrent(m_win);
	glfwSwapInterval(1);
}

Window::~Window() {
	glfwDestroyWindow(m_win);
}

bool Window::render() {

	if (glfwWindowShouldClose(m_win)) {
		return false;
	}

	glfwSwapBuffers(m_win);
	glfwPollEvents();

	return true;

}

} // namespace engine
