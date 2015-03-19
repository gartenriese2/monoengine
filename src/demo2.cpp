#include "demo2.hpp"

#include "engine/gl/shader.hpp"
#include "engine/core/log.hpp"
#include "engine/extern/imgui.h"

#include <chrono>
#include <random>
#include <future>

constexpr auto k_maxNumObjects = 1000u;
constexpr auto k_initialNumObjects = 300u;
constexpr auto k_avg = 20u;

std::default_random_engine generator2;
std::uniform_real_distribution<float> distribution2(0.f, 1.f);

Demo2::Demo2(const glm::uvec2 & size)
  : m_engine{size, "monoEngine Demo2", true},
	m_prog{"Demo2 prog"},
	m_fbo{"Demo2 fbo"},
	m_colorTex{"Demo2 color tex"},
	m_depthTex{"Demo2 depth tex"},
	m_ibo{"Demo2 ibo"},
	m_modelMatrixBuffer{"Demo2 ssbo"},
	m_vao{"Demo2 vao"},
	m_numObjects{k_initialNumObjects},
	m_rotate{true}
{
	m_engine.showFPS(true);
	init(size);
}

void Demo2::init(const glm::uvec2 & size) {

	m_cam.setRatio(static_cast<float>(size.x) / static_cast<float>(size.y));
	m_cam.setFov(glm::radians(45.f));
	m_cam.translate({0.f, 0.f, 3.f});

	// shader
#ifdef _WIN32
	gl::Shader vert("../shader/test/instancedraw2.vert", "instance_vert");
	gl::Shader frag("../shader/test/color.frag", "color_frag");
#else
	gl::Shader vert("shader/test/instancedraw2.vert", "instance_vert");
	gl::Shader frag("shader/test/color.frag", "color_frag");
#endif
	m_prog.attachShader(vert);
	m_prog.attachShader(frag);

	// ibo
	std::vector<GLushort> idx = {
		19, 18, 16,
		16, 17, 19,

		7, 3, 1,
		1, 5, 7,

		22, 23, 21,
		21, 20, 22,

		2, 6, 4,
		4, 0, 2,

		9, 8, 12,
		12, 13, 9,

		14, 15, 11,
		11, 10, 14
	};
	m_ibo.createImmutableStorage(static_cast<unsigned int>(idx.size() * sizeof(GLushort)), 0, idx.data());

	// vao
	m_vao.bind();
	m_vao.bindElementBuffer(m_ibo);

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
	m_modelMatrixBuffer.createImmutableStorage(k_maxNumObjects * k_maxNumObjects
			* static_cast<unsigned int>(sizeof(glm::mat4)), GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
	m_objects.resize(k_maxNumObjects * k_maxNumObjects);
	orderModels();
	setModelMatrices();

	m_colorTex.createImmutableStorage(size.x, size.y, GL_RGBA32F);
	m_depthTex.createImmutableStorage(size.x, size.y, GL_DEPTH_COMPONENT32F);
	m_fbo.attachTexture(GL_COLOR_ATTACHMENT0, m_colorTex, 0);
	m_fbo.attachTexture(GL_DEPTH_ATTACHMENT, m_depthTex, 0);
	if (!m_fbo.isComplete(GL_FRAMEBUFFER)) {
		LOG_WARNING("Demo2 has incomplete fbo!");
	}

}

void Demo2::orderModels() {
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

void Demo2::setModelMatrices() {
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

double Demo2::getAverageMs(const std::deque<GLuint64> & deque) {
	auto avg = 0.0;
	for (const auto & t : deque) {
		avg += static_cast<double>(t) * 0.000001;
	}
	avg /= static_cast<double>(deque.size());
	return avg;
}

double Demo2::getAverageMs(const std::deque<double> & deque) {
	auto avg = 0.0;
	for (const auto & t : deque) {
		avg += t;
	}
	avg /= static_cast<double>(deque.size());
	return avg;
}

bool Demo2::render() {

	const auto start = std::chrono::system_clock::now();
	m_timer.start();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// setup shader
	m_prog.use();
	m_prog["col"] = glm::vec3{1.f, 0.f, 0.f};
	m_prog["ViewProj"] = m_cam.getProjMatrix() * m_cam.getViewMatrix();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_modelMatrixBuffer);

	// draw
	m_fbo.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_fbo.draw({GL_COLOR_ATTACHMENT0});
	m_vao.bind();
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0,
			static_cast<GLsizei>(m_numObjects * m_numObjects));
	m_fbo.unbind();

	const auto size = static_cast<glm::ivec2>(m_engine.getWindowPtr()->getFrameBufferSize());
	m_fbo.blitAttachment(GL_COLOR_ATTACHMENT0, {0, 0, size.x, size.y});

	// rotate objects
	if (m_rotate) {
		const auto t = std::thread::hardware_concurrency();
		if (t <= 1) {
			for (auto i = 0u; i < m_numObjects * m_numObjects; ++i) {
				m_objects[i].rotate(0.03f,
				{ distribution2(generator2), distribution2(generator2), distribution2(generator2) });
				m_objects[i].rotateAround(0.01f, { 0.f, 1.f, 0.f });
			}
		}
		else {
			const auto parFunc = [&](unsigned int from, unsigned int to) {
				for (auto i = from; i < to; ++i) {
					m_objects[i].rotate(0.03f,
						{ distribution2(generator2), distribution2(generator2), distribution2(generator2) });
					m_objects[i].rotateAround(0.01f, { 0.f, 1.f, 0.f });
				}
			};
			std::vector<std::future<void>> futures;
			futures.reserve(t);
			const auto step = m_numObjects * m_numObjects / t;
			for (auto j = 0u; j < t; ++j) {
				futures.emplace_back(std::async(std::launch::async, parFunc, j * step, j == t - 1 ? m_numObjects * m_numObjects : (j + 1) * step));
			}
			for (const auto & f : futures) {
				f.wait();
			}
		}
		setModelMatrices();
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
	ImGui::Begin("Demo2");
	int numObj = static_cast<int>(m_numObjects);
	ImGui::SliderInt("numObjects", &numObj, 1, k_maxNumObjects);
	if (m_numObjects != static_cast<unsigned int>(numObj)) {
		m_numObjects = static_cast<unsigned int>(numObj);
		orderModels();
		setModelMatrices();
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
	ImGui::Checkbox("Rotate", &m_rotate);
	ImGui::End();

	return m_engine.render();

}
