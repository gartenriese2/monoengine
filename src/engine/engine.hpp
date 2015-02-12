#ifndef _ENGINE_
#define _ENGINE_

#include "window.hpp"

#include <memory>

namespace engine {

class Engine {

	public:

		Engine(unsigned int, unsigned int, const std::string &, bool = false);
		~Engine();

		bool render();

	private:

		void initGLFW();
		void initGL();
		void initDebugging();

		std::unique_ptr<Window> m_window;

};

} // namespace engine

#endif // _ENGINE_
