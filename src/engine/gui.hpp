#pragma once

#include "window.hpp"
#include "core/input.hpp"

#include <memory>
#include <map>

namespace engine {

class Engine;

class Gui {

	public:

		Gui(std::unique_ptr<Window> & window, std::unique_ptr<core::Input> & input);
		Gui(const Gui &) = delete;
		Gui(Gui &&) = delete;
		Gui & operator=(const Gui &) = delete;
		Gui & operator=(Gui &&) = delete;
		~Gui();

		void newFrame();

	protected:

		friend Engine;
		void update();
		bool render();

	private:

		void initFontTexture();
		void initVBO();
		void initVAO();
		void initProgram();

		std::unique_ptr<Window> & m_window;
		std::unique_ptr<core::Input> & m_input;

		glm::dvec2 m_mousePos;
		bool m_leftMouseButtonDown;
		bool m_leftMouseButtonRelease;
		double m_scrollOffset;

		std::map<int, bool> m_keysPressed;
		std::map<int, bool> m_keysReleased;
		bool m_controlPressed;
		bool m_shiftPressed;

		bool m_frameInitialized;

};

static_assert(!std::is_move_constructible<Gui>(), "Should not be MoveConstructible");
static_assert(!std::is_copy_constructible<Gui>(), "Should not be CopyConstructible");
static_assert(!std::is_move_assignable<Gui>(), "Should not be MoveAssignable");
static_assert(!std::is_copy_assignable<Gui>(), "Should not be CopyAssignable");

} // namespace engine
