#include "engine.hpp"

#include "extern/glfwinclude.hpp"
#include "extern/glinclude.hpp"
#include "extern/imgui.h"
#include "core/log.hpp"

namespace engine {

constexpr auto k_avg = 20u;

Engine::Engine(const glm::uvec2 & size,	const std::string & title, const bool debugging)
  : m_start{std::chrono::system_clock::now()},
	m_currentFPS{0u},
	m_showFPS{false}
{

	initGLFW();
	m_window = std::make_unique<Window>(size, title);
	glfwSetWindowUserPointer(m_window->getGLFWWindow(), this);
	initGL();

	if (debugging && glewIsSupported("GL_ARB_debug_output")) initDebugging();

	m_input = std::make_unique<core::Input>(m_window->getGLFWWindow());
	m_gui = std::make_unique<Gui>(m_window, m_input);

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

void Engine::initGL() {

	glewExperimental = GL_TRUE;
	const auto err = glewInit();
	if (err != GLEW_OK) {
		LOG_ERROR("Failed to initialize GLEW:", glewGetErrorString(err));
	}

}

#ifndef _WIN32
const std::string formatDebugOutput(GLenum, GLenum,	GLuint, GLenum, const std::string &);
void debugCallback(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar *, const void *);

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
#else
void Engine::initDebugging() {
	LOG_WARNING("Windows does not support OpenGL debugging");
}
#endif

bool Engine::render() {

	if (m_showFPS) {
		renderFPS();
	}
	ImGui::Render();
	if (m_window->render()) {
		const std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - m_start;
		m_times.emplace_back(elapsed.count() * 1000.0);
		m_start = std::chrono::system_clock::now();
		m_gui->update();
		return true;
	}
	return false;

}

void Engine::showFPS(const bool show) {
	m_showFPS = show;
}

void Engine::renderFPS() {
	bool opened;
	if (!ImGui::Begin("Example: Fixed Overlay", &opened, ImVec2(100,20), 0.3f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings)) {
		ImGui::End();
		return;
	}

	if (m_times.size() == k_avg) {
		auto avg = 0.0;
		for (const auto & t : m_times) {
			avg += t;
		}
		avg /= static_cast<double>(k_avg);
		m_currentFPS = static_cast<unsigned int>(1000.0 / avg);
		m_times.erase(m_times.begin(), m_times.begin() + k_avg / 2);
	}

	const auto size = m_window->getFrameBufferSize();
	ImGui::SetWindowPos(ImVec2(static_cast<float>(size.x) - 110.f, 10.f));
	ImGui::Text("fps: %d", m_currentFPS);
	ImGui::End();
}

} // namespace engine
