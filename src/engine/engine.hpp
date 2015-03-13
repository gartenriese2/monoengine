#pragma once

#include "window.hpp"
#include "core/input.hpp"
#include "gui.hpp"

#include <memory>
#include <chrono>
#include <deque>

namespace engine {

class Engine {

	public:

		Engine(const glm::uvec2 &, const std::string &, bool = false);
		~Engine();

		std::unique_ptr<Window> & getWindowPtr() { return m_window; }
		std::unique_ptr<core::Input> & getInputPtr() { return m_input; }
		std::unique_ptr<Gui> & getGuiPtr() { return m_gui; }

		bool render();
		void showFPS(bool);

	private:

		void initGLFW();
		void initGL();
		void initDebugging();

		void renderFPS();

		std::unique_ptr<Window> m_window;
		std::unique_ptr<core::Input> m_input;
		std::unique_ptr<Gui> m_gui;

		std::chrono::time_point<std::chrono::system_clock> m_start;
		std::deque<double> m_times;
		unsigned int m_currentFPS;
		bool m_showFPS;

};

} // namespace engine
