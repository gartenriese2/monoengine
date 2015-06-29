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

		Engine(const glm::uvec2 & size, const std::string & title, bool debugging = false);
		Engine(const Engine &) = delete;
		Engine(Engine && other) noexcept;
		Engine & operator=(const Engine &) = delete;
		Engine & operator=(Engine &&) & = default;
		~Engine();

		auto & getWindowPtr() noexcept { return m_window; }
		auto & getInputPtr() noexcept { return m_input; }
		auto & getGuiPtr() noexcept { return m_gui; }

		bool render();
		void showFPS(bool show) noexcept;

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

static_assert(std::is_nothrow_move_constructible<Engine>(), "Should be noexcept MoveConstructible");
static_assert(!std::is_copy_constructible<Engine>(), "Should not be CopyConstructible");
static_assert(std::is_nothrow_move_assignable<Engine>(), "Should be noexcept MoveAssignable");
static_assert(!std::is_copy_assignable<Engine>(), "Should not be CopyAssignable");

} // namespace engine
