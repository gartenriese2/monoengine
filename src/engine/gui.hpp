#pragma once

namespace engine {

class Gui {

	public:

		Gui(unsigned int, unsigned int);
		~Gui();

		void update();
		void render();

	private:

		void initFontTexture();
		void initVBO();
		void initVAO();
		void initProgram();

};

} // namespace engine
