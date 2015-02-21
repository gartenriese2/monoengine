#include "input.hpp"

#include "../engine.hpp"
#include "log.hpp"

namespace core {

Input::Input(GLFWwindow * const window)
  : m_window{window}
{

	glfwSetKeyCallback(m_window, [](GLFWwindow * windowPtr, int key, int scancode, int action, int mods){

		auto * const win = static_cast<engine::Engine *>(glfwGetWindowUserPointer(windowPtr));
		auto & inputPtr = win->getInputPtr();

		for (const auto & func : inputPtr->m_keyFuncs) {
			func(key, scancode, action, mods);
		}

	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow * windowPtr, double xpos, double ypos){

		auto * const win = static_cast<engine::Engine *>(glfwGetWindowUserPointer(windowPtr));
		auto & inputPtr = win->getInputPtr();

		for (const auto & func : inputPtr->m_mouseCursorFuncs) {
			func(xpos, ypos);
		}

	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow * windowPtr, int button, int action, int mods){

		auto * const win = static_cast<engine::Engine *>(glfwGetWindowUserPointer(windowPtr));
		auto & inputPtr = win->getInputPtr();

		for (const auto & func : inputPtr->m_mouseButtonFuncs) {
			func(button, action, mods);
		}

	});

	glfwSetScrollCallback(window, [](GLFWwindow * windowPtr, double xoffset, double yoffset){

		auto * const win = static_cast<engine::Engine *>(glfwGetWindowUserPointer(windowPtr));
		auto & inputPtr = win->getInputPtr();

		for (const auto & func : inputPtr->m_mouseScrollFuncs) {
			func(xoffset, yoffset);
		}

	});

}

void Input::addKeyFunc(const std::function<void(int, int, int, int)> & func) {
	m_keyFuncs.emplace_back(func);
}

void Input::addMouseCursorFunc(const std::function<void(double, double)> & func) {
	m_mouseCursorFuncs.emplace_back(func);
}

void Input::addMouseButtonFunc(const std::function<void(int, int, int)> & func) {
	m_mouseButtonFuncs.emplace_back(func);
}

void Input::addMouseScrollFunc(const std::function<void(double, double)> & func) {
	m_mouseScrollFuncs.emplace_back(func);
}

} // namespace core
