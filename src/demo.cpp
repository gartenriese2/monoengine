#include "demo.hpp"

#include "engine/gl/shader.hpp"

constexpr auto k_numTrianglesRoot = 10u;
constexpr auto k_numTriangles = k_numTrianglesRoot * k_numTrianglesRoot;

Demo::Demo(const unsigned int width, const unsigned int height)
  : m_engine{width, height, "monoEngine Demo"},
	m_basicProg{"basic prog"},
	m_vboTriangle{"vboTriangle"},
	m_vboCube{"vboCube"},
	m_iboTriangle{"iboTriangle"},
	m_iboCube{"iboCube"},
	m_vaoTriangle{"vaoTriangle"},
	m_vaoTriangleIndex{"vaoTriangleIndex"},
	m_vaoCubeIndex{"vaoCubeIndex"},
	m_shouldClose{false}
{
	m_cam.setRatio(static_cast<float>(width) / static_cast<float>(height));
	m_cam.setFov(glm::radians(45.f));
	m_cam.translate({0.f, 0.f, 5.f});

	initShaders();
	initBuffers();
	initVAOs();
	initFunctions();
}

void Demo::initShaders() {
	gl::Shader basic_vert("shader/test/basicdraw.vert", "basic_vert");
	gl::Shader multi_vert("shader/test/multidraw.vert", "multi_vert");
	gl::Shader instance_vert("shader/test/instancedraw.vert", "instance_vert");
	gl::Shader frag("shader/test/color.frag", "color_frag");

	m_basicProg.attachShader(basic_vert);
	m_basicProg.attachShader(frag);
}

void Demo::initBuffers() {

	const auto stepSize = 2.f / static_cast<float>(k_numTrianglesRoot);
	const auto delta = stepSize / 4.f;
	std::vector<GLfloat> vec;
	std::vector<GLushort> idx;

	vec.reserve(k_numTriangles * 6);
	for (auto i = -1.f; i < 1.f; i += stepSize) {
		for (auto j = -1.f; j < 1.f; j += stepSize) {
			vec.emplace_back(i);
			vec.emplace_back(j);
			vec.emplace_back(i + stepSize);
			vec.emplace_back(j);
			vec.emplace_back(i + stepSize / 2.f);
			vec.emplace_back(j + stepSize);
		}
	}
	m_vboTriangle.createStorage(static_cast<unsigned int>(vec.size()) * sizeof(float), 0, vec.data());

	idx.reserve(k_numTriangles * 3);
	for (auto i = 0u; i < k_numTriangles; ++i) {
		idx.emplace_back(0);
		idx.emplace_back(1);
		idx.emplace_back(2);
	}
	m_iboTriangle.createStorage(static_cast<unsigned int>(idx.size()) * sizeof(GLushort), 0, idx.data());

	vec.clear();
	vec.reserve(k_numTriangles * 24);
	for (auto i = -1.f; i < 1.f; i += stepSize) {
		for (auto j = -1.f; j < 1.f; j += stepSize) {
			vec.emplace_back(i + delta);
			vec.emplace_back(j + delta);
			vec.emplace_back(stepSize - delta);

			vec.emplace_back(i + stepSize - delta);
			vec.emplace_back(j + delta);
			vec.emplace_back(stepSize - delta);

			vec.emplace_back(i + stepSize - delta);
			vec.emplace_back(j + stepSize - delta);
			vec.emplace_back(stepSize - delta);

			vec.emplace_back(i + delta);
			vec.emplace_back(j + stepSize - delta);
			vec.emplace_back(stepSize - delta);

			vec.emplace_back(i + delta);
			vec.emplace_back(j + delta);
			vec.emplace_back(delta - stepSize);

			vec.emplace_back(i + stepSize - delta);
			vec.emplace_back(j + delta);
			vec.emplace_back(delta - stepSize);

			vec.emplace_back(i + stepSize - delta);
			vec.emplace_back(j + stepSize - delta);
			vec.emplace_back(delta - stepSize);

			vec.emplace_back(i + delta);
			vec.emplace_back(j + stepSize - delta);
			vec.emplace_back(delta - stepSize);
		}
	}
	m_vboCube.createStorage(static_cast<unsigned int>(vec.size()) * sizeof(float), 0, vec.data());

}

void Demo::initVAOs() {

	glBindVertexArray(m_vaoTriangle);
	m_vaoTriangle.enableAttribBinding(0);
	m_vaoTriangle.bindVertexBuffer(m_vboTriangle, 0, 0, 2 * sizeof(float), 0);
	m_vaoTriangle.bindVertexFormat(0, 2, GL_FLOAT, GL_FALSE, 0);

	glBindVertexArray(m_vaoTriangleIndex);
	m_vaoTriangleIndex.enableAttribBinding(0);
	m_vaoTriangleIndex.bindVertexBuffer(m_vboTriangle, 0, 0, 2 * sizeof(float), 0);
	m_vaoTriangleIndex.bindVertexFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
	m_vaoTriangleIndex.bindElementBuffer(m_iboTriangle);

	glBindVertexArray(m_vaoCubeIndex);
	m_vaoCubeIndex.enableAttribBinding(0);
	m_vaoCubeIndex.bindVertexBuffer(m_vboCube, 0, 0, 3 * sizeof(float), 0);
	m_vaoCubeIndex.bindVertexFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	m_vaoCubeIndex.bindElementBuffer(m_iboCube);

}

void Demo::initFunctions() {

	m_funcs.emplace(RenderType::BASIC, [&](){
		m_basicProg["MVP"] = m_cam.getProjMatrix() * m_cam.getViewMatrix();
		for (auto i = 0u; i < k_numTriangles; ++i) {
			glDrawArrays(GL_TRIANGLES, static_cast<GLint>(i) * 3, 3);
		}
	});

	m_funcs.emplace(RenderType::BASICINDEX, [&](){
		m_basicProg["MVP"] = m_cam.getProjMatrix() * m_cam.getViewMatrix();
		for (auto i = 0u; i < k_numTriangles; ++i) {
			glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0, static_cast<GLint>(i) * 3);
		}
	});

}

double Demo::getAverageMs(const std::deque<GLuint64> & deque) {
	auto avg = 0.0;
	for (const auto & t : deque) {
		avg += static_cast<long double>(t) * 0.000001;
	}
	avg /= deque.size();
	return avg;
}

void Demo::use(const RenderType type) {

	switch (type) {
		case RenderType::BASIC:
			m_basicProg.use();
			m_basicProg["col"] = glm::vec3{1.f, 0.f, 0.f};
			m_renderFunc = m_funcs.at(type);
			glBindVertexArray(m_vaoTriangle);
			break;
		case RenderType::BASICINDEX:
			m_basicProg.use();
			m_basicProg["col"] = glm::vec3{1.f, 0.f, 0.f};
			m_renderFunc = m_funcs.at(type);
			glBindVertexArray(m_vaoTriangleIndex);
			break;
	}

}

void Demo::render() {

	glClear(GL_COLOR_BUFFER_BIT);

	m_timer.start();

	m_renderFunc();

	m_timeDeque.emplace_back(m_timer.stop());
	if (m_timeDeque.size() == 100) {
		const auto ms = getAverageMs(m_timeDeque);
		m_timeDeque.erase(m_timeDeque.begin(), m_timeDeque.begin() + 50);
		LOG("Time: " + std::to_string(ms) + " ms");
	}

	m_cam.translateLocal({0.1f, 0.f, 0.f});
	m_cam.yaw(glm::radians(1.f));

	if (!m_engine.render()) {
		m_shouldClose = true;
	}

}
