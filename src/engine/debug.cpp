#include "debug.hpp"

#include "gl/glinclude.hpp"

namespace engine {

Debug * Debug::s_instance = nullptr;

Debug & Debug::instance() {

	static Cleanup cleanup;
	if (s_instance == nullptr) {
		s_instance = new Debug();
	}

	return * s_instance;

}

Debug::Cleanup::~Cleanup() {

	delete Debug::s_instance;
	Debug::s_instance = nullptr;

}

void Debug::logGL() {

	auto err = glGetError();
	switch(err) {
		case GL_NO_ERROR:
			log("GL_NO_ERROR");
			break;
		case GL_INVALID_ENUM:
			log("GL_INVALID_ENUM");
			break;
		case GL_INVALID_VALUE:
			log("GL_INVALID_VALUE");
			break;
		case GL_INVALID_OPERATION:
			log("GL_INVALID_OPERATION");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			log("GL_INVALID_FRAMEBUFFER_OPERATION");
			break;
		case GL_OUT_OF_MEMORY:
			log("GL_OUT_OF_MEMORY");
			break;
		default:
			log("NO STANDARD ERROR");
	}

}

} // namespace engine
