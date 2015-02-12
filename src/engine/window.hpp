#ifndef _WINDOW_
#define _WINDOW_

#include "glfwinclude.hpp"

#include <string>

namespace engine {

class Window {

	public:

		Window() : m_win{nullptr} {}
		Window(unsigned int, unsigned int, const std::string &);
		~Window();

		bool render();

	private:

		GLFWwindow * m_win;

};

} // namespace engine

#endif // _WINDOW_
