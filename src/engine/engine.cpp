#include "engine.hpp"

#include "extern/glfwinclude.hpp"
#include "extern/glinclude.hpp"
#include "core/log.hpp"

namespace engine {

Engine::Engine(const unsigned int width, const unsigned int height,
		const std::string & title, const bool debugging) {

	initGLFW();
	m_window = std::make_unique<Window>(width, height, title);
	m_input = std::make_unique<core::Input>(m_window->getGLFWWindow());
	glfwSetWindowUserPointer(m_window->getGLFWWindow(), this);
	initGL();

	if (debugging && glewIsSupported("GL_ARB_debug_output")) initDebugging();

	LOG("Engine initialized!");

}

Engine::~Engine() {

	glfwTerminate();
	LOG("Engine shut down!");

}

void Engine::initGLFW() {

	if (!glfwInit()) {
		LOG_ERROR("Could not initialize GLFW!");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

void Engine::initGL() {

	glewExperimental = GL_TRUE;
	const auto err = glewInit();
    if (err != GLEW_OK) {
        LOG_ERROR("Failed to initialize GLEW:", glewGetErrorString(err));
    }

}

const std::string formatDebugOutput(GLenum source, GLenum type,
	GLuint id, GLenum severity, const std::string & msg) {

	std::string sourceStr {"UNDEFINED"};
	switch (source) {
		case GL_DEBUG_SOURCE_API:
			sourceStr = "API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			sourceStr = "WINDOW_SYSTEM";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			sourceStr = "SHADER_COMPILER";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			sourceStr = "THIRD_PARTY";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			sourceStr = "APPLICATION";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			sourceStr = "OTHER";
			break;
	}

	std::string typeStr {"UNDEFINED"};
	switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			typeStr = "ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			typeStr = "DEPRECATED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			typeStr = "UNDEFINED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			typeStr = "PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			typeStr = "PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_MARKER:
			typeStr = "MARKER";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			typeStr = "PUSH_GROUP";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			typeStr = "POP_GROUP";
			break;
		case GL_DEBUG_TYPE_OTHER:
			typeStr = "OTHER";
			break;
	}

	std::string severityStr {"UNDEFINED"};
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			severityStr = "HIGH";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			severityStr = "MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_LOW:
			severityStr = "LOW";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			severityStr = "NOTIFICATION";
			break;
	}

	return "OpenGL: " + msg + " [source=" + sourceStr + " type="
		+ typeStr + " severity=" + severityStr + " id=" + std::to_string(id) + "]";

}

void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
					 GLsizei, const GLchar * message, const void *) {

	const auto output = formatDebugOutput(source, type, id, severity, message);
	LOG(output);

}

void Engine::initDebugging() {

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	glDebugMessageCallback(debugCallback, stderr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

}

bool Engine::render() {

	return m_window->render();

}

} // namespace engine
