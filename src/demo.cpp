#include "demo.hpp"

#include "engine/gl/shader.hpp"
#include "engine/core/log.hpp"
#include "engine/extern/imgui.h"
#include "engine/object.hpp"

constexpr auto k_maxNumObjects = 2000u;
constexpr auto k_initialNumObjects = 900u;

Demo::Demo(const glm::uvec2 & size)
  : m_engine{size, "monoEngine Demo", true},
	m_prog{"demo prog"},
	m_vbo{"demo vbo"},
	m_ibo{"demo ibo"},
	m_modelMatrixBuffer{"demo ssbo"},
	m_vao{"demo vao"},
	m_numObjects{k_initialNumObjects}
{
	m_cam.setRatio(static_cast<float>(size.x) / static_cast<float>(size.y));
	m_cam.setFov(glm::radians(45.f));
	m_cam.translate({0.f, 0.f, 5.f});

	init();
}

void Demo::init() {

	// shader
	gl::Shader vert("shader/test/instancedraw.vert", "instance_vert");
	gl::Shader frag("shader/test/color.frag", "color_frag");
	m_prog.attachShader(vert);
	m_prog.attachShader(frag);

	// vbo
	std::vector<GLfloat> vec = {
		-1.f, -1.f, 1.f,
		1.f, -1.f, 1.f,
		1.f, 1.f, 1.f,
		-1.f, 1.f, 1.f,
		-1.f, -1.f, -1.f,
		1.f, -1.f, -1.f,
		1.f, 1.f, -1.f,
		-1.f, 1.f, -1.f
	};
	m_vbo.createImmutableStorage(static_cast<unsigned int>(vec.size()) * sizeof(GLfloat), 0, vec.data());

	// ibo
	std::vector<GLubyte> idx = {
		0, 1, 2,
		0, 2, 3,

		4, 0, 3,
		4, 3, 7,

		5, 4, 7,
		5, 7, 6,

		1, 5, 6,
		1, 6, 2,

		3, 2, 6,
		3, 6, 7,

		4, 5, 1,
		4, 1, 0
	};
	m_ibo.createImmutableStorage(static_cast<unsigned int>(vec.size()) * sizeof(GLubyte), 0, idx.data());

	// vao
	m_vao.bind();
	m_vao.enableAttribBinding(0);
	m_vao.bindVertexBuffer(0, m_vbo, 0, 3 * sizeof(float));
	m_vao.bindVertexFormat(0, 0, 3, GL_FLOAT, GL_FALSE, 0);
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
	m_modelMatrixBuffer.createImmutableStorage(k_maxNumObjects * k_maxNumObjects * sizeof(glm::mat4),
			GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
	setModelMatrices();

}

void Demo::setModelMatrices() {
	const auto scale = 2.f / m_numObjects;
	void * voidPtr = m_modelMatrixBuffer.map(0, m_numObjects, GL_MAP_WRITE_BIT);
	auto * ptr = reinterpret_cast<glm::mat4 *>(voidPtr);
	for (auto i = 0u; i < m_numObjects * m_numObjects; ++i) {
		engine::Object o;
		o.scale({scale * 0.25f, scale * 0.25f, scale * 0.25f});
		const auto y = i / m_numObjects;
		const auto x = i % m_numObjects;
		o.moveTo({-1.f + scale * (x + 0.5f), -1.f + scale * (y + 0.5f), 0.f});
		*ptr = o.getModelMatrix();
		++ptr;
	}
	m_modelMatrixBuffer.unmap();
}

double Demo::getAverageMs(const std::deque<GLuint64> & deque) {
	auto avg = 0.0;
	for (const auto & t : deque) {
		avg += static_cast<long double>(t) * 0.000001;
	}
	avg /= deque.size();
	return avg;
}

bool Demo::render() {

	glClear(GL_COLOR_BUFFER_BIT);

	m_timer.start();

	m_prog.use();
	m_prog["col"] = glm::vec3{1.f, 0.f, 0.f};
	m_prog["ViewProj"] = m_cam.getProjMatrix() * m_cam.getViewMatrix();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_modelMatrixBuffer);

	m_vao.bind();
	const auto instances = static_cast<GLsizei>(m_numObjects * m_numObjects);
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0, instances);

	m_timeDeque.emplace_back(m_timer.stop());
	static auto ms = 0.0;
	if (m_timeDeque.size() == 10) {
		ms = getAverageMs(m_timeDeque);
		m_timeDeque.erase(m_timeDeque.begin(), m_timeDeque.begin() + 5);
	}

	// m_cam.translateLocal({0.1f, 0.f, 0.f});
	// m_cam.yaw(glm::radians(1.f));

	// Gui
	m_engine.getGuiPtr()->update();
	int tmp = static_cast<int>(m_numObjects);
	ImGui::SliderInt("numObjects", &tmp, 1, k_maxNumObjects);
	if (m_numObjects != static_cast<unsigned int>(tmp)) {
		m_numObjects = static_cast<unsigned int>(tmp);
		setModelMatrices();
	}
	ImGui::Columns(2, "time", true);
	ImGui::Text("ms");
	ImGui::NextColumn();
	ImGui::Text("%f", ms);
	ImGui::NextColumn();
	ImGui::Text("fps");
	ImGui::NextColumn();
	ImGui::Text("%d", static_cast<unsigned int>(1000.0 / ms));
	ImGui::NextColumn();
	m_engine.getGuiPtr()->render();

	return m_engine.render();

}
