#include "demo.hpp"

#include "engine/gl/shader.hpp"
#include "engine/core/log.hpp"
#include "engine/extern/imgui.h"

#include <chrono>
#include <random>

constexpr auto k_initialNumObjects = 5u;
constexpr auto k_avg = 20u;
constexpr auto k_uboBinding = 0u;

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0.f, 1.f);

Demo::Demo(const glm::uvec2 & size)
  : m_engine{size, "monoEngine Demo", true},
	m_numObjects{k_initialNumObjects},
	m_rotate{true}
{
	m_engine.showFPS(true);
	init(size);
}

void Demo::init(const glm::uvec2 & size) {

	m_cam.setRatio(static_cast<float>(size.x) / static_cast<float>(size.y));
	m_cam.setFov(glm::radians(45.f));
	m_cam.translate({0.f, 0.f, 5.f});

	// shader
	GLint maxBufferSize;
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxBufferSize);
	maxBufferSize /= static_cast<GLint>(sizeof(glm::mat4));
	m_maxNumObjects = static_cast<unsigned int>(std::sqrt(maxBufferSize));
	gl::Shader vert(GL_VERTEX_SHADER);
	vert.addSourceFromString("#version 330 core\n");
	vert.addSourceFromString("const int NUM_MATRICES = " + std::to_string(maxBufferSize) + ";\n");
#ifdef _WIN32
	vert.addSourceFromFile("../shader/test/instancedraw.vert");
#else
	vert.addSourceFromFile("shader/test/instancedraw.vert");
#endif
	if (!vert.compileSource()) {
		LOG_ERROR("could not compile vertex shader!");
	}
#ifdef _WIN32
	gl::Shader frag("../shader/test/color.frag");
#else
	gl::Shader frag("shader/test/color.frag");
#endif
	m_prog.attachShader(vert);
	m_prog.attachShader(frag);

	// vbo (pos3 and norm3 interleaved)
	std::vector<GLfloat> vec = {
		-1.f, -1.f, 1.f,
		0.f, 0.f, 1.f,
		1.f, -1.f, 1.f,
		0.f, 0.f, 1.f,
		1.f, 1.f, 1.f,
		0.f, 0.f, 1.f,
		-1.f, 1.f, 1.f,
		0.f, 0.f, 1.f,

		-1.f, -1.f, -1.f,
		-1.f, 0.f, 0.f,
		-1.f, -1.f, 1.f,
		-1.f, 0.f, 0.f,
		-1.f, 1.f, 1.f,
		-1.f, 0.f, 0.f,
		-1.f, 1.f, -1.f,
		-1.f, 0.f, 0.f,

		1.f, -1.f, -1.f,
		0.f, 0.f, -1.f,
		-1.f, -1.f, -1.f,
		0.f, 0.f, -1.f,
		-1.f, 1.f, -1.f,
		0.f, 0.f, -1.f,
		1.f, 1.f, -1.f,
		0.f, 0.f, -1.f,

		1.f, -1.f, 1.f,
		1.f, 0.f, 0.f,
		1.f, -1.f, -1.f,
		1.f, 0.f, 0.f,
		1.f, 1.f, -1.f,
		1.f, 0.f, 0.f,
		1.f, 1.f, 1.f,
		1.f, 0.f, 0.f,

		-1.f, 1.f, 1.f,
		0.f, 1.f, 0.f,
		1.f, 1.f, 1.f,
		0.f, 1.f, 0.f,
		1.f, 1.f, -1.f,
		0.f, 1.f, 0.f,
		-1.f, 1.f, -1.f,
		0.f, 1.f, 0.f,

		1.f, -1.f, -1.f,
		0.f, -1.f, 0.f,
		-1.f, -1.f, -1.f,
		0.f, -1.f, 0.f,
		-1.f, -1.f, 1.f,
		0.f, -1.f, 0.f,
		1.f, -1.f, 1.f,
		0.f, -1.f, 0.f
	};
	m_vbo.bind(GL_ARRAY_BUFFER);
	m_vbo.createMutableStorage(static_cast<unsigned int>(vec.size() * sizeof(GLfloat)),
			GL_STATIC_DRAW, vec.data());
	m_vbo.unbind();

	// ibo
	std::vector<GLushort> idx = {
		 0,  1,  2,
		 2,  3,  0,

		 4,  5,  6,
		 6,  7,  4,

		 8,  9, 10,
		10, 11,  8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};
	m_ibo.bind(GL_ARRAY_BUFFER);
	m_ibo.createMutableStorage(static_cast<unsigned int>(vec.size() * sizeof(GLushort)),
			GL_STATIC_DRAW, idx.data());
	m_ibo.unbind();

	// vao
	m_vao.bind();
	m_vbo.bind(GL_ARRAY_BUFFER);
	m_vao.enableAttribBinding(0);
	m_vao.enableAttribBinding(1);
	m_vao.bindVertexBuffer(0, m_vbo, 0, 6 * sizeof(GLfloat));
	m_vao.bindVertexFormat(0, 0, 3, GL_FLOAT, GL_FALSE, 0);
	m_vao.bindVertexFormat(0, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
	m_ibo.bind(GL_ELEMENT_ARRAY_BUFFER);
	m_vao.unbind();
	m_vbo.unbind();
	m_ibo.unbind();

	// keys
	m_engine.getInputPtr()->addKeyFunc([&](const int key, const int, const int action, const int mods){
		if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			if ((mods & GLFW_MOD_SHIFT) != 0) {
				m_cam.translateLocal({0.f, 0.f, -0.5f});
			} else {
				m_cam.translateLocal({0.f, 0.f, -0.05f});
			}
		}
		if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			if ((mods & GLFW_MOD_SHIFT) != 0) {
				m_cam.translateLocal({0.f, 0.f, 0.5f});
			} else {
				m_cam.translateLocal({0.f, 0.f, 0.05f});
			}
		}
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(m_engine.getWindowPtr()->getGLFWWindow(), GL_TRUE);
		}
	});

	// modelMatrixBuffer
	m_modelMatrixBuffer.bind(GL_UNIFORM_BUFFER);
	m_modelMatrixBuffer.createMutableStorage(m_maxNumObjects * m_maxNumObjects
			* static_cast<unsigned int>(sizeof(glm::mat4)),	GL_DYNAMIC_DRAW);
	m_objects.resize(m_maxNumObjects * m_maxNumObjects);
	orderModels();
	setModelMatrices();
	m_modelMatrixBuffer.unbind();

	m_colorTex.bind();
	m_colorTex.createImmutableStorage(size.x, size.y, GL_RGBA32F);
	m_depthTex.bind();
	m_depthTex.createImmutableStorage(size.x, size.y, GL_DEPTH_COMPONENT32F);
	m_fbo.bind();
	m_fbo.attachTexture(GL_COLOR_ATTACHMENT0, m_colorTex, 0);
	m_fbo.attachTexture(GL_DEPTH_ATTACHMENT, m_depthTex, 0);
	if (!m_fbo.isComplete()) {
		LOG_WARNING("demo has incomplete fbo!");
	}
	m_fbo.unbind();
}

void Demo::orderModels() {
	const auto scale = 2.f / static_cast<float>(m_numObjects);
	auto i = 0u;
	for (auto & obj : m_objects) {
		obj.resetScale();
		obj.resetMoves();
		obj.scale({scale * 0.33f, scale * 0.33f, scale * 0.33f});
		const auto y = i / m_numObjects;
		const auto x = i % m_numObjects;
		obj.moveTo({-1.f + scale * (static_cast<float>(x) + 0.5f),
				-1.f + scale * (static_cast<float>(y) + 0.5f), 0.f});
		i++;
	}
}

void Demo::setModelMatrices() {
	void * voidPtr = m_modelMatrixBuffer.map(0, m_numObjects * m_numObjects, GL_MAP_WRITE_BIT);
	auto * ptr = reinterpret_cast<glm::mat4 *>(voidPtr);
	auto count = 0u;
	for (const auto & obj : m_objects) {
		*ptr = obj.getModelMatrix();
		++ptr;
		if (++count > m_numObjects * m_numObjects) break;
	}
	m_modelMatrixBuffer.unmap();
}

double Demo::getAverageMs(const std::deque<GLuint64> & deque) {
	auto avg = 0.0;
	for (const auto & t : deque) {
		avg += static_cast<double>(t) * 0.000001;
	}
	avg /= static_cast<double>(deque.size());
	return avg;
}

double Demo::getAverageMs(const std::deque<double> & deque) {
	auto avg = 0.0;
	for (const auto & t : deque) {
		avg += t;
	}
	avg /= static_cast<double>(deque.size());
	return avg;
}

bool Demo::render() {

	const auto start = std::chrono::system_clock::now();
	m_timer.start();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// setup shader
	m_prog.use();
	m_prog["col"] = glm::vec3{1.f, 0.f, 0.f};
	m_prog["ViewProj"] = m_cam.getProjMatrix() * m_cam.getViewMatrix();

	// ubo
	const auto index = glGetUniformBlockIndex(static_cast<GLuint>(m_prog), "ModelMatrixBuffer");
	glUniformBlockBinding(static_cast<GLuint>(m_prog), index, k_uboBinding);
	glBindBufferBase(GL_UNIFORM_BUFFER, k_uboBinding, m_modelMatrixBuffer);

	// draw
	m_fbo.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_fbo.draw({GL_COLOR_ATTACHMENT0});
	m_vao.bind();
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0,
			static_cast<GLsizei>(m_numObjects * m_numObjects));
	m_vao.unbind();
	m_fbo.unbind();

	m_fbo.bind(GL_READ_FRAMEBUFFER);
	glDrawBuffer(GL_BACK);
	const auto size = static_cast<glm::ivec2>(m_engine.getWindowPtr()->getFrameBufferSize());
	m_fbo.blitAttachment(GL_COLOR_ATTACHMENT0, {0, 0, size.x, size.y});
	m_fbo.unbind();

	// rotate objects
	if (m_rotate) {
		for (auto i = 0u; i < m_numObjects * m_numObjects; ++i) {
			m_objects[i].rotate(0.03f,
					{distribution(generator), distribution(generator), distribution(generator)});
			m_objects[i].rotateAround(0.01f, {0.f, 1.f, 0.f});
		}
		m_modelMatrixBuffer.bind(GL_UNIFORM_BUFFER);
		setModelMatrices();
		m_modelMatrixBuffer.unbind();
	}

	// stop timers
	m_timeDeque.emplace_back(m_timer.stop());
	static auto ms = 0.0;
	if (m_timeDeque.size() == k_avg) {
		ms = getAverageMs(m_timeDeque);
		m_timeDeque.erase(m_timeDeque.begin(), m_timeDeque.begin() + k_avg / 2);
	}
	std::chrono::duration<double> tmp = std::chrono::system_clock::now() - start;
	m_cpuTimeDeque.emplace_back(tmp.count() * 1000.0);
	static auto ms_cpu = 0.0;
	if (m_cpuTimeDeque.size() == k_avg) {
		ms_cpu = getAverageMs(m_cpuTimeDeque);
		m_cpuTimeDeque.erase(m_cpuTimeDeque.begin(), m_cpuTimeDeque.begin() + k_avg / 2);
	}

	// Gui
	m_engine.getGuiPtr()->newFrame();
	ImGui::Begin("Demo");
	int numObj = static_cast<int>(m_numObjects);
	ImGui::SliderInt("numObjects", &numObj, 1, static_cast<int>(m_maxNumObjects));
	if (m_numObjects != static_cast<unsigned int>(numObj)) {
		m_numObjects = static_cast<unsigned int>(numObj);
		orderModels();
		m_modelMatrixBuffer.bind(GL_UNIFORM_BUFFER);
		setModelMatrices();
		m_modelMatrixBuffer.unbind();
	}
	ImGui::Columns(2, "time", true);
	ImGui::Text("ms gpu");
	ImGui::NextColumn();
	ImGui::Text("%f", ms);
	ImGui::NextColumn();
	ImGui::Text("ms cpu");
	ImGui::NextColumn();
	ImGui::Text("%f", ms_cpu);
	ImGui::NextColumn();
	ImGui::End();

	return m_engine.render();

}
