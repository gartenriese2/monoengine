#pragma once

#include "window.hpp"
#include "core/input.hpp"
#include "gui.hpp"

#include <memory>

namespace engine {

class Engine {

	public:

		Engine(const glm::uvec2 &, const std::string &, bool = false);
		~Engine();

		std::unique_ptr<core::Input> & getInputPtr() { return m_input; }
		std::unique_ptr<Gui> & getGuiPtr() { return m_gui; }

		bool render();

	private:

		void initGLFW();
		void initGL();
		void initDebugging();

		std::unique_ptr<Window> m_window;
		std::unique_ptr<core::Input> m_input;
		std::unique_ptr<Gui> m_gui;

};

} // namespace engine
