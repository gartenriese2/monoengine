#pragma once

#include "extern/glfwinclude.hpp"

#include <string>
#include <glm/glm.hpp>

namespace engine {

class Window {

	public:

		Window() : m_win{nullptr} {}
		Window(const glm::uvec2 &, const std::string &);
		~Window();

		GLFWwindow * getGLFWWindow() { return m_win; }
		const glm::uvec2 getScreenCoordSize() const { return m_screenCoordSize; }
		const glm::uvec2 getFrameBufferSize() const { return m_frameBufferSize; }

		bool render();

	private:

		GLFWwindow * m_win;

		glm::uvec2 m_screenCoordSize;
		glm::uvec2 m_frameBufferSize;

};

} // namespace engine
