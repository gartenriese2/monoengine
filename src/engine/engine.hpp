#pragma once

#include "window.hpp"
#include "core/input.hpp"

#include <memory>

namespace engine {

class Engine {

	public:

		Engine(unsigned int, unsigned int, const std::string &, bool = false);
		~Engine();

		std::unique_ptr<core::Input> & getInputPtr() { return m_input; }

		bool render();

	private:

		void initGLFW();
		void initGL();
		void initDebugging();

		std::unique_ptr<Window> m_window;
		std::unique_ptr<core::Input> m_input;

};

} // namespace engine
