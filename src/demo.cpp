#include "demo.hpp"

#include "engine/gl/shader.hpp"
#include "engine/core/log.hpp"

constexpr auto k_numObjectsRoot = 100u;
constexpr auto k_numObjects = k_numObjectsRoot * k_numObjectsRoot;

Demo::Demo(const unsigned int width, const unsigned int height)
  : m_engine{width, height, "monoEngine Demo"},
	m_basicProg{"basic prog"},
	m_vboTriangle{"vboTriangle"},
	m_vboCube{"vboCube"},
	m_vboCubeIndexed{"vboCubeIndexed"},
	m_iboTriangle{"iboTriangle"},
	m_iboCube{"iboCube"},
	m_vaoTriangle{"vaoTriangle"},
	m_vaoTriangleIndex{"vaoTriangleIndex"},
	m_vaoCube{"vaoCube"},
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

	const auto stepSize = 2.f / static_cast<float>(k_numObjectsRoot);
	const auto delta = stepSize / 4.f;
	std::vector<GLfloat> vec;
	std::vector<GLushort> idx;
	std::vector<GLuint> idxInt;

	/*
	 *	vboTriangle
	 */

	vec.reserve(k_numObjects * 6);
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

	/*
	 *	iboTriangle
	 */

	idx.reserve(k_numObjects * 3);
	for (auto i = 0u; i < k_numObjects; ++i) {
		idx.emplace_back(0);
		idx.emplace_back(1);
		idx.emplace_back(2);
	}
	m_iboTriangle.createStorage(static_cast<unsigned int>(idx.size()) * sizeof(GLushort), 0, idx.data());

	/*
	 *	vboCubeIndexed
	 */

	vec.clear();
	vec.reserve(k_numObjects * 24);
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
	m_vboCubeIndexed.createStorage(static_cast<unsigned int>(vec.size()) * sizeof(float), 0, vec.data());

	/*
	 *	iboCube
	 */

	idx.clear();
	idx.reserve(k_numObjects * 36);
	for (auto i = 0u; i < k_numObjects; ++i) {
		idx.emplace_back(0);
		idx.emplace_back(1);
		idx.emplace_back(2);
		idx.emplace_back(0);
		idx.emplace_back(2);
		idx.emplace_back(3);

		idx.emplace_back(4);
		idx.emplace_back(0);
		idx.emplace_back(3);
		idx.emplace_back(4);
		idx.emplace_back(3);
		idx.emplace_back(7);

		idx.emplace_back(5);
		idx.emplace_back(4);
		idx.emplace_back(7);
		idx.emplace_back(5);
		idx.emplace_back(7);
		idx.emplace_back(6);

		idx.emplace_back(1);
		idx.emplace_back(5);
		idx.emplace_back(6);
		idx.emplace_back(1);
		idx.emplace_back(6);
		idx.emplace_back(2);

		idx.emplace_back(3);
		idx.emplace_back(2);
		idx.emplace_back(6);
		idx.emplace_back(3);
		idx.emplace_back(6);
		idx.emplace_back(7);

		idx.emplace_back(4);
		idx.emplace_back(5);
		idx.emplace_back(1);
		idx.emplace_back(4);
		idx.emplace_back(1);
		idx.emplace_back(0);
	}
	m_iboCube.createStorage(static_cast<unsigned int>(idx.size()) * sizeof(GLushort), 0, idx.data());

	/*
	 *	vboCube
	 */

	auto empl = [&vec](const glm::vec3 v){
		vec.emplace_back(v.x);
		vec.emplace_back(v.y);
		vec.emplace_back(v.z);
	};

	vec.clear();
	vec.reserve(k_numObjects * 108);
	for (auto i = -1.f; i < 1.f; i += stepSize) {
		for (auto j = -1.f; j < 1.f; j += stepSize) {
			std::vector<glm::vec3> vertices;
			vertices.emplace_back(i + delta, j + delta, stepSize / 2.f - delta);
			vertices.emplace_back(i + stepSize - delta, j + delta, stepSize / 2.f - delta);
			vertices.emplace_back(i + stepSize - delta, j + stepSize - delta, stepSize / 2.f - delta);
			vertices.emplace_back(i + delta, j + stepSize - delta, stepSize / 2.f - delta);
			vertices.emplace_back(vertices[0] - glm::vec3{0.f, 0.f, stepSize + 2.f * delta});
			vertices.emplace_back(vertices[1] - glm::vec3{0.f, 0.f, stepSize + 2.f * delta});
			vertices.emplace_back(vertices[2] - glm::vec3{0.f, 0.f, stepSize + 2.f * delta});
			vertices.emplace_back(vertices[3] - glm::vec3{0.f, 0.f, stepSize + 2.f * delta});

			empl(vertices[0]);
			empl(vertices[1]);
			empl(vertices[2]);
			empl(vertices[0]);
			empl(vertices[2]);
			empl(vertices[3]);

			empl(vertices[4]);
			empl(vertices[0]);
			empl(vertices[3]);
			empl(vertices[4]);
			empl(vertices[3]);
			empl(vertices[7]);

			empl(vertices[5]);
			empl(vertices[4]);
			empl(vertices[7]);
			empl(vertices[5]);
			empl(vertices[7]);
			empl(vertices[6]);

			empl(vertices[1]);
			empl(vertices[5]);
			empl(vertices[6]);
			empl(vertices[1]);
			empl(vertices[6]);
			empl(vertices[2]);

			empl(vertices[3]);
			empl(vertices[2]);
			empl(vertices[6]);
			empl(vertices[3]);
			empl(vertices[6]);
			empl(vertices[7]);

			empl(vertices[4]);
			empl(vertices[5]);
			empl(vertices[1]);
			empl(vertices[4]);
			empl(vertices[1]);
			empl(vertices[0]);

		}
	}
	m_vboCube.createStorage(static_cast<unsigned int>(vec.size()) * sizeof(float), 0, vec.data());

	/*
	 *	multi
	 */

	m_multiOffsetsTriangle.reserve(k_numObjects);
	m_multiCountsTriangle.reserve(k_numObjects);
	for (auto i = 0u; i < k_numObjects; ++i) {
		m_multiOffsetsTriangle.emplace_back(i * 3);
		m_multiCountsTriangle.emplace_back(3);
	}

	m_multiOffsetsCube.reserve(k_numObjects);
	m_multiCountsCube.reserve(k_numObjects);
	for (auto i = 0u; i < k_numObjects; ++i) {
		m_multiOffsetsCube.emplace_back(i * 108);
		m_multiCountsCube.emplace_back(108);
	}

	/*
	 *	multi indexed
	 */

 	m_multiOffsetsTriangleIndexed.reserve(k_numObjects);
	m_multiCountsTriangleIndexed.reserve(k_numObjects);
	m_multiBaseVertexTriangle.reserve(k_numObjects);
	for (auto i = 0u; i < k_numObjects; ++i) {
		m_multiOffsetsTriangleIndexed.emplace_back(static_cast<GLchar *>(0) + (i * 3 * sizeof(GLuint)));
		m_multiCountsTriangleIndexed.emplace_back(3);
		m_multiBaseVertexTriangle.emplace_back(i * 3);
	}


	idxInt.reserve(k_numObjects * 3);
	for (auto i = 0u; i < k_numObjects; ++i) {
		idxInt.emplace_back(0);
		idxInt.emplace_back(1);
		idxInt.emplace_back(2);
	}
	m_iboMultiTriangle.createStorage(static_cast<unsigned int>(idxInt.size()) * sizeof(GLuint), 0, idxInt.data());

	m_multiOffsetsCubeIndexed.reserve(k_numObjects);
	m_multiCountsCubeIndexed.reserve(k_numObjects);
	m_multiBaseVertexCube.reserve(k_numObjects);
	for (auto i = 0u; i < k_numObjects; ++i) {
		m_multiOffsetsCubeIndexed.emplace_back(static_cast<GLchar *>(0) + (i * 36 * sizeof(GLuint)));
		m_multiCountsCubeIndexed.emplace_back(36);
		m_multiBaseVertexCube.emplace_back(i * 8);
	}

	idxInt.clear();
	idxInt.reserve(k_numObjects * 36);
	for (auto i = 0u; i < k_numObjects; ++i) {
		idxInt.emplace_back(0);
		idxInt.emplace_back(1);
		idxInt.emplace_back(2);
		idxInt.emplace_back(0);
		idxInt.emplace_back(2);
		idxInt.emplace_back(3);

		idxInt.emplace_back(4);
		idxInt.emplace_back(0);
		idxInt.emplace_back(3);
		idxInt.emplace_back(4);
		idxInt.emplace_back(3);
		idxInt.emplace_back(7);

		idxInt.emplace_back(5);
		idxInt.emplace_back(4);
		idxInt.emplace_back(7);
		idxInt.emplace_back(5);
		idxInt.emplace_back(7);
		idxInt.emplace_back(6);

		idxInt.emplace_back(1);
		idxInt.emplace_back(5);
		idxInt.emplace_back(6);
		idxInt.emplace_back(1);
		idxInt.emplace_back(6);
		idxInt.emplace_back(2);

		idxInt.emplace_back(3);
		idxInt.emplace_back(2);
		idxInt.emplace_back(6);
		idxInt.emplace_back(3);
		idxInt.emplace_back(6);
		idxInt.emplace_back(7);

		idxInt.emplace_back(4);
		idxInt.emplace_back(5);
		idxInt.emplace_back(1);
		idxInt.emplace_back(4);
		idxInt.emplace_back(1);
		idxInt.emplace_back(0);
	}
	m_iboMultiCube.createStorage(static_cast<unsigned int>(idxInt.size()) * sizeof(GLuint), 0, idxInt.data());

}

void Demo::initVAOs() {

	glBindVertexArray(m_vaoTriangle);
	m_vaoTriangle.enableAttribBinding(0);
	m_vaoTriangle.bindVertexBuffer(m_vboTriangle, 0, 0, 2 * sizeof(float), 0);
	m_vaoTriangle.bindVertexFormat(0, 2, GL_FLOAT, GL_FALSE, 0);

	glBindVertexArray(m_vaoCube);
	m_vaoCube.enableAttribBinding(0);
	m_vaoCube.bindVertexBuffer(m_vboCube, 0, 0, 3 * sizeof(float), 0);
	m_vaoCube.bindVertexFormat(0, 3, GL_FLOAT, GL_FALSE, 0);

	glBindVertexArray(m_vaoTriangleIndex);
	m_vaoTriangleIndex.enableAttribBinding(0);
	m_vaoTriangleIndex.bindVertexBuffer(m_vboTriangle, 0, 0, 2 * sizeof(float), 0);
	m_vaoTriangleIndex.bindVertexFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
	m_vaoTriangleIndex.bindElementBuffer(m_iboTriangle);

	glBindVertexArray(m_vaoCubeIndex);
	m_vaoCubeIndex.enableAttribBinding(0);
	m_vaoCubeIndex.bindVertexBuffer(m_vboCubeIndexed, 0, 0, 3 * sizeof(float), 0);
	m_vaoCubeIndex.bindVertexFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	m_vaoCubeIndex.bindElementBuffer(m_iboCube);

	glBindVertexArray(m_vaoTriangleIndexMulti);
	m_vaoTriangleIndexMulti.enableAttribBinding(0);
	m_vaoTriangleIndexMulti.bindVertexBuffer(m_vboTriangle, 0, 0, 2 * sizeof(float), 0);
	m_vaoTriangleIndexMulti.bindVertexFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
	m_vaoTriangleIndexMulti.bindElementBuffer(m_iboMultiTriangle);

	glBindVertexArray(m_vaoCubeIndexMulti);
	m_vaoCubeIndexMulti.enableAttribBinding(0);
	m_vaoCubeIndexMulti.bindVertexBuffer(m_vboCubeIndexed, 0, 0, 3 * sizeof(float), 0);
	m_vaoCubeIndexMulti.bindVertexFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	m_vaoCubeIndexMulti.bindElementBuffer(m_iboMultiCube);

}

void Demo::initFunctions() {

	m_funcs.emplace(RenderType::BASIC_TRIANGLE, [&](){
		m_basicProg["MVP"] = m_cam.getProjMatrix() * m_cam.getViewMatrix();
		for (auto i = 0u; i < k_numObjects; ++i) {
			glDrawArrays(GL_TRIANGLES, static_cast<GLint>(i) * 3, 3);
		}
	});

	m_funcs.emplace(RenderType::BASIC_CUBE, [&](){
		m_basicProg["MVP"] = m_cam.getProjMatrix() * m_cam.getViewMatrix();
		for (auto i = 0u; i < k_numObjects; ++i) {
			glDrawArrays(GL_TRIANGLES, static_cast<GLint>(i) * 108, 108);
		}
	});

	m_funcs.emplace(RenderType::BASICINDEX_TRIANGLE, [&](){
		m_basicProg["MVP"] = m_cam.getProjMatrix() * m_cam.getViewMatrix();
		for (auto i = 0u; i < k_numObjects; ++i) {
			glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0, static_cast<GLint>(i) * 3);
		}
	});

	m_funcs.emplace(RenderType::BASICINDEX_CUBE, [&](){
		m_basicProg["MVP"] = m_cam.getProjMatrix() * m_cam.getViewMatrix();
		for (auto i = 0u; i < k_numObjects; ++i) {
			glDrawElementsBaseVertex(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0, static_cast<GLint>(i) * 8);
		}
	});

	m_funcs.emplace(RenderType::MULTI_TRIANGLE, [&](){
		m_basicProg["MVP"] = m_cam.getProjMatrix() * m_cam.getViewMatrix();
		glMultiDrawArrays(GL_TRIANGLES, m_multiOffsetsTriangle.data(), m_multiCountsTriangle.data(), k_numObjects);
	});

	m_funcs.emplace(RenderType::MULTI_CUBE, [&](){
		m_basicProg["MVP"] = m_cam.getProjMatrix() * m_cam.getViewMatrix();
		glMultiDrawArrays(GL_TRIANGLES, m_multiOffsetsCube.data(), m_multiCountsCube.data(), k_numObjects);
	});

	m_funcs.emplace(RenderType::MULTIINDEX_TRIANGLE, [&](){
		m_basicProg["MVP"] = m_cam.getProjMatrix() * m_cam.getViewMatrix();
		glMultiDrawElementsBaseVertex(GL_TRIANGLES, m_multiCountsTriangleIndexed.data(), GL_UNSIGNED_INT,
				reinterpret_cast<const void * const *>(m_multiOffsetsTriangleIndexed.data()),
				k_numObjects, m_multiBaseVertexTriangle.data());
	});

	m_funcs.emplace(RenderType::MULTIINDEX_CUBE, [&](){
		m_basicProg["MVP"] = m_cam.getProjMatrix() * m_cam.getViewMatrix();
		glMultiDrawElementsBaseVertex(GL_TRIANGLES, m_multiCountsCubeIndexed.data(), GL_UNSIGNED_INT,
				reinterpret_cast<const void * const *>(m_multiOffsetsCubeIndexed.data()),
				k_numObjects, m_multiBaseVertexCube.data());
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
		case RenderType::BASIC_TRIANGLE:
			m_basicProg.use();
			m_basicProg["col"] = glm::vec3{1.f, 0.f, 0.f};
			m_renderFunc = m_funcs.at(type);
			glBindVertexArray(m_vaoTriangle);
			break;
		case RenderType::BASIC_CUBE:
			m_basicProg.use();
			m_basicProg["col"] = glm::vec3{1.f, 0.f, 0.f};
			m_renderFunc = m_funcs.at(type);
			glBindVertexArray(m_vaoCube);
			break;
		case RenderType::BASICINDEX_TRIANGLE:
			m_basicProg.use();
			m_basicProg["col"] = glm::vec3{1.f, 0.f, 0.f};
			m_renderFunc = m_funcs.at(type);
			glBindVertexArray(m_vaoTriangleIndex);
			break;
		case RenderType::BASICINDEX_CUBE:
			m_basicProg.use();
			m_basicProg["col"] = glm::vec3{1.f, 0.f, 0.f};
			m_renderFunc = m_funcs.at(type);
			glBindVertexArray(m_vaoCubeIndex);
			break;
		case RenderType::MULTI_TRIANGLE:
			m_basicProg.use();
			m_basicProg["col"] = glm::vec3{1.f, 0.f, 0.f};
			m_renderFunc = m_funcs.at(type);
			glBindVertexArray(m_vaoTriangle);
			break;
		case RenderType::MULTI_CUBE:
			m_basicProg.use();
			m_basicProg["col"] = glm::vec3{1.f, 0.f, 0.f};
			m_renderFunc = m_funcs.at(type);
			glBindVertexArray(m_vaoCube);
			break;
		case RenderType::MULTIINDEX_TRIANGLE:
			m_basicProg.use();
			m_basicProg["col"] = glm::vec3{1.f, 0.f, 0.f};
			m_renderFunc = m_funcs.at(type);
			glBindVertexArray(m_vaoTriangleIndexMulti);
			break;
		case RenderType::MULTIINDEX_CUBE:
			m_basicProg.use();
			m_basicProg["col"] = glm::vec3{1.f, 0.f, 0.f};
			m_renderFunc = m_funcs.at(type);
			glBindVertexArray(m_vaoCubeIndexMulti);
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
