#pragma once

#include "extern/glfwinclude.hpp"

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <functional>

namespace engine {

class Window {

	public:

		Window() : m_win{nullptr} {}
		Window(const glm::uvec2 & size, const std::string & name);
		Window(const Window &) = delete;
		Window(Window && other) = delete;
		Window & operator=(const Window &) = delete;
		Window & operator=(Window && other) = delete;
		~Window();

		auto * getGLFWWindow() noexcept { return m_win; }
		auto getScreenCoordSize() const noexcept { return m_screenCoordSize; }
		auto getFrameBufferSize() const noexcept { return m_frameBufferSize; }

		void addWindowSizeFunc(const std::function<void(int, int)> & func);
		void addFrameBufferSizeFunc(const std::function<void(int, int)> & func);

		bool render();

	private:

		GLFWwindow * m_win;

		glm::uvec2 m_screenCoordSize;
		glm::uvec2 m_frameBufferSize;

		std::vector<std::function<void(int, int)>> m_windowSizeFuncs;
		std::vector<std::function<void(int, int)>> m_frameBufferSizeFuncs;

};

static_assert(!std::is_move_constructible<Window>(), "Should not be MoveConstructible");
static_assert(!std::is_copy_constructible<Window>(), "Should not be CopyConstructible");
static_assert(!std::is_move_assignable<Window>(), "Should not be MoveAssignable");
static_assert(!std::is_copy_assignable<Window>(), "Should not be CopyAssignable");

} // namespace engine
