#include "window.hpp"

#include "core/log.hpp"
#include "engine.hpp"

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

	glfwSetWindowSizeCallback(m_win, [](GLFWwindow * winPtr, int width, int height){
		auto * const win {static_cast<engine::Engine *>(glfwGetWindowUserPointer(winPtr))};
		auto & windowPtr {win->getWindowPtr()};
		windowPtr->m_screenCoordSize = {width, height};

		for (const auto & func : windowPtr->m_windowSizeFuncs) {
			func(width, height);
		}
	});
	glfwSetFramebufferSizeCallback(m_win, [](GLFWwindow * winPtr, int width, int height){
		auto * const win {static_cast<engine::Engine *>(glfwGetWindowUserPointer(winPtr))};
		auto & windowPtr {win->getWindowPtr()};
		windowPtr->m_frameBufferSize = {width, height};

		for (const auto & func : windowPtr->m_frameBufferSizeFuncs) {
			func(width, height);
		}
	});
}

Window::~Window() {
	glfwDestroyWindow(m_win);
}

void Window::addWindowSizeFunc(const std::function<void(int, int)> & func) {
	m_windowSizeFuncs.emplace_back(func);
}

void Window::addFrameBufferSizeFunc(const std::function<void(int, int)> & func) {
	m_frameBufferSizeFuncs.emplace_back(func);
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
