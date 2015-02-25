#pragma once

#include "window.hpp"
#include "core/input.hpp"

#include <memory>
#include <map>

namespace engine {

class Gui {

	public:

		Gui(std::unique_ptr<Window> &, std::unique_ptr<core::Input> &);
		~Gui();

		void update();
		void render();

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

};

} // namespace engine
