#pragma once

#include "window.hpp"
#include "core/input.hpp"

#include <memory>

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

};

} // namespace engine
