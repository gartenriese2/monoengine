#include "log.hpp"

#include "../extern/glinclude.hpp"

namespace core {

Log * Log::s_instance = nullptr;

Log & Log::instance() {

	static Cleanup cleanup;
	if (s_instance == nullptr) {
		s_instance = new Log();
	}

	return * s_instance;

}

Log::Cleanup::~Cleanup() {

	delete Log::s_instance;
	Log::s_instance = nullptr;

}

void Log::logGL() {

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

} // namespace core
