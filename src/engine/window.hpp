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
		Window(const glm::uvec2 &, const std::string &);
		~Window();

		GLFWwindow * getGLFWWindow() { return m_win; }
		const glm::uvec2 getScreenCoordSize() const { return m_screenCoordSize; }
		const glm::uvec2 getFrameBufferSize() const { return m_frameBufferSize; }

		void addWindowSizeFunc(const std::function<void(int, int)> &);
		void addFrameBufferSizeFunc(const std::function<void(int, int)> &);

		bool render();

	private:

		GLFWwindow * m_win;

		glm::uvec2 m_screenCoordSize;
		glm::uvec2 m_frameBufferSize;

		std::vector<std::function<void(int, int)>> m_windowSizeFuncs;
		std::vector<std::function<void(int, int)>> m_frameBufferSizeFuncs;

};

} // namespace engine
