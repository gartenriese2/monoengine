#include "window.hpp"

#include "core/log.hpp"

namespace engine {

Window::Window(const glm::uvec2 & size, const std::string & name)
  : m_win{glfwCreateWindow(static_cast<int>(size.x), static_cast<int>(size.y),
			name.c_str(), NULL, NULL)},
	m_screenCoordSize{size}
{
	if (!m_win) {
		LOG_ERROR("Could not create GLFW Window!");
	}
	glfwMakeContextCurrent(m_win);
	glfwSwapInterval(1);

	int w, h;
	glfwGetFramebufferSize(m_win, &w, &h);
	m_frameBufferSize = {w, h};
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
