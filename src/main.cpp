#include <cstdlib>
#include <vector>
#include <algorithm>
#include <deque>

#include "engine/engine.hpp"
#include "engine/gl/shader.hpp"
#include "engine/gl/program.hpp"
#include "engine/gl/buffer.hpp"
#include "engine/gl/vertexarray.hpp"
#include "engine/gl/timer.hpp"

#include "engine/core/camera.hpp"

constexpr auto NUM_TRIANGLES = 10u;

void vertexPullingDemo() {

	struct Segment
	{
		Segment(GLintptr b, GLintptr p, GLintptr e)
		  : begin{b}, ptr{p}, end{e} {}
		GLintptr begin;
		GLintptr ptr;
		GLintptr end;
	};

	std::vector<Segment>    m_freelist;
	std::vector<Segment>    m_used;

	auto align = [](GLsizei alignment, GLsizeiptr size, GLintptr& ptr, GLsizeiptr space) {
		bool success = false;
		if (size <= space) {
			GLintptr ptr2 = ((ptr + alignment - 1) / alignment) * alignment;
			GLsizeiptr diff = static_cast<GLsizeiptr>(ptr2 - ptr);
			if (diff <= space - size) {
				ptr = ptr2;
				space -= diff;
				success = true;
			}
		}
		return success;
	};

	auto alloc = [&](const GLsizeiptr size, const GLsizei alignment) {
		GLintptr offset = 0;
		std::vector<Segment>::iterator it;
		const auto it_end = m_freelist.end();
		for (it = m_freelist.begin(); it != it_end; ++it) {
			GLsizeiptr space = static_cast<GLsizeiptr>(it->end - it->begin);

			GLintptr ptr = it->begin;
			// first fit
			if (align(alignment, size, ptr, space)) {
				offset = ptr;
				Segment used(it->begin, ptr, ptr + size);

				it->begin = used.end;
				it->ptr = used.end;
				if (it->begin == it->end) {
					m_freelist.erase(it);
				}

				auto pos = std::lower_bound(m_used.begin(), m_used.end(),
						used, [] (const Segment& s0, const Segment& s1) -> bool
						{
							return s0.begin < s1.begin;
						});
				m_used.insert(pos, std::move(used));

				break;
			}
		}
		if (it == it_end)
			abort();

		return offset;
	};

	// instance manager stuff
	gl::Buffer instanceBuffer("instance buffer");
	constexpr int MAX_NUM_INSTANCES = 12000;
	instanceBuffer.createStorage(MAX_NUM_INSTANCES, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
	//auto instanceDataPtr = instanceBuffer.getDataPointer(MAX_NUM_INSTANCES,
	//          GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	// mesh manager stuff
	gl::Buffer meshDataBuffer("vertex shader storage buffer");
	meshDataBuffer.createStorage(268435456, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
	m_freelist.emplace_back(0, 0, 268435456);
	gl::Buffer meshPoolBuffer("mesh pool");
	constexpr int MAX_NUM_MESHES = 1200;
	meshPoolBuffer.createStorage(MAX_NUM_MESHES, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
	//auto meshPoolDataPtr = meshPoolBuffer.getDataPointer(MAX_NUM_MESHES,
	//          GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	// initVAO
	GLsizei stride = static_cast<GLsizei>(3 * sizeof(float));
	gl::VertexArray vao("vao");
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, meshDataBuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
	auto tmpOffset = 3 * sizeof(float);
	if (static_cast<GLsizei>(tmpOffset) != stride) {
		LOG("static_cast<GLsizei>(tmpOffset) != stride");
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshDataBuffer);
	glBindVertexArray(0);

	// add mesh
	GLenum index_type;
	GLsizeiptr per_index_size;
	per_index_size = sizeof(GLubyte);
	index_type = GL_UNSIGNED_BYTE;
	GLsizeiptr per_vertex_size = 3 * sizeof(float);

	const GLsizeiptr vertices_size = per_vertex_size * 3;
	const GLsizeiptr indices_size = per_index_size * 3;
	const GLsizeiptr size = vertices_size + indices_size;

	const auto offset = alloc(size, static_cast<GLsizei>(vertices_size));
	if (offset % vertices_size != 0) {
		LOG("offset % vertices_size != 0");
	}

	// upload to gpu
	void * ptr = glMapNamedBufferRange(meshDataBuffer,
				offset, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
	float* data = static_cast<float*>(ptr);

	*data++ = -1.f;
	*data++ = -1.f;
	*data++ = 0.f;
	*data++ = 1.f;
	*data++ = -1.f;
	*data++ = 0.f;
	*data++ = 0.f;
	*data++ = 1.f;
	*data++ = 0.f;

	GLubyte* indices = reinterpret_cast<GLubyte*>(data);
	*indices = static_cast<GLubyte>(0);
	indices += per_index_size;
	*indices = static_cast<GLubyte>(1);
	indices += per_index_size;
	*indices = static_cast<GLubyte>(2);
	indices += per_index_size;

	if (indices - static_cast<GLubyte*>(ptr) != size) {
		LOG("indices - static_cast<GLubyte*>(ptr) != size");
	}
	glUnmapNamedBuffer(meshDataBuffer);

	// Add MeshStruct on GPU
	//const GLintptr mesh_offset = m_mesh_pool.alloc();
	//const GLuint mesh_index = m_mesh_pool.offsetToIndex(mesh_offset);
	//auto* mesh_data = m_mesh_pool.offsetToPointer(mesh_offset);

	gl::Shader vert("shader/test/triangle.vert", "triangle_vert");
	gl::Shader frag("shader/test/red.frag", "triangle_frag");
	gl::Program prog("test prog");
	prog.attachShader(vert);
	prog.attachShader(frag);
	prog.use();

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);

}

std::vector<GLfloat> createVertexPositionDataTriangle(unsigned int num) {

	std::vector<GLfloat> vec;
	vec.reserve(num * num * 6);

	const auto stepSize = 2.f / static_cast<float>(num);
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

	return vec;

}

std::vector<GLfloat> createVertexPositionDataCubeForArrays(unsigned int num) {

	std::vector<GLfloat> vec;
	vec.reserve(num * num * 108);

	auto empl = [&vec](const glm::vec3 v){
		vec.emplace_back(v.x);
		vec.emplace_back(v.y);
		vec.emplace_back(v.z);
	};

	const auto stepSize = 2.f / static_cast<float>(num);
	const auto delta = stepSize / 4.f;
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

	return vec;

}

std::vector<GLfloat> createVertexPositionDataCube(unsigned int num) {

	std::vector<GLfloat> vec;
	vec.reserve(num * num * 24);

	const auto stepSize = 2.f / static_cast<float>(num);
	const auto delta = stepSize / 4.f;
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

	return vec;

}

std::vector<GLushort> createVertexIndexDataCube(unsigned int num) {

	std::vector<GLushort> vec;
	vec.reserve(num * num * 36);

	for (auto i = 0u; i < num * num; ++i) {
		vec.emplace_back(0);
		vec.emplace_back(1);
		vec.emplace_back(2);
		vec.emplace_back(0);
		vec.emplace_back(2);
		vec.emplace_back(3);

		vec.emplace_back(4);
		vec.emplace_back(0);
		vec.emplace_back(3);
		vec.emplace_back(4);
		vec.emplace_back(3);
		vec.emplace_back(7);

		vec.emplace_back(5);
		vec.emplace_back(4);
		vec.emplace_back(7);
		vec.emplace_back(5);
		vec.emplace_back(7);
		vec.emplace_back(6);

		vec.emplace_back(1);
		vec.emplace_back(5);
		vec.emplace_back(6);
		vec.emplace_back(1);
		vec.emplace_back(6);
		vec.emplace_back(2);

		vec.emplace_back(3);
		vec.emplace_back(2);
		vec.emplace_back(6);
		vec.emplace_back(3);
		vec.emplace_back(6);
		vec.emplace_back(7);

		vec.emplace_back(4);
		vec.emplace_back(5);
		vec.emplace_back(1);
		vec.emplace_back(4);
		vec.emplace_back(1);
		vec.emplace_back(0);
	}

	return vec;

}

auto getAverageMs(const std::deque<GLuint64> & deque) {
	auto avg = 0.0;
	for (const auto & t : deque) {
		avg += static_cast<long double>(t) * 0.000001;
	}
	avg /= deque.size();
	return avg;
}

void instancingElementDemo(engine::Engine & e) {

	gl::Timer timer;
	std::deque<GLuint64> timeDeque;
	constexpr auto primcount = NUM_TRIANGLES * NUM_TRIANGLES;

	// shader
	gl::Shader vert("shader/test/instancedraw.vert", "instance_vert");
	gl::Shader frag("shader/test/color.frag", "color_frag");
	gl::Program prog("instance prog");
	prog.attachShader(vert);
	prog.attachShader(frag);

	// vbo
	const auto dif = 2.f / static_cast<float>(NUM_TRIANGLES);
	const std::vector<GLfloat> tri = {
		-1.f, -1.f,
		-1.f + dif, -1.f,
		-1.f + dif / 2.f, -1.f + dif
	};
	gl::Buffer vbo("Instance Draw VBO");
	vbo.createStorage(static_cast<unsigned int>(tri.size()) * sizeof(float), 0, tri.data());

	// ibo
	std::vector<GLubyte> idx = {
		0, 1, 2
	};
	gl::Buffer ibo("Instance Draw IBO");
	ibo.createStorage(static_cast<unsigned int>(idx.size()) * sizeof(GLubyte), 0, idx.data());

	// vao
	gl::VertexArray vao("Multi Draw VAO");
	glBindVertexArray(vao);
	vao.enableAttribBinding(0);
	vao.bindVertexBuffer(vbo, 0, 0, 2 * sizeof(float), 0);
	vao.bindVertexFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
	vao.bindElementBuffer(ibo);

	prog.use();
	prog["col"] = glm::vec3{1.f, 0.f, 0.f};
	prog["width"] = NUM_TRIANGLES;
	while (e.render()) {
		timer.start();

		glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, 0, primcount);

		timeDeque.emplace_back(timer.stop());
		if (timeDeque.size() == 100) {
			const auto ms = getAverageMs(timeDeque);
			timeDeque.erase(timeDeque.begin(), timeDeque.begin() + 50);
			LOG("Time: " + std::to_string(ms) + " ms");
		}
	}

}

void instancingDemo(engine::Engine & e) {

	gl::Timer timer;
	std::deque<GLuint64> timeDeque;
	constexpr auto primcount = NUM_TRIANGLES * NUM_TRIANGLES;

	// shader
	gl::Shader vert("shader/test/instancedraw.vert", "instance_vert");
	gl::Shader frag("shader/test/color.frag", "color_frag");
	gl::Program prog("instance prog");
	prog.attachShader(vert);
	prog.attachShader(frag);

	// vbo
	const auto dif = 2.f / static_cast<float>(NUM_TRIANGLES);
	const std::vector<GLfloat> tri = {
		-1.f, -1.f,
		-1.f + dif, -1.f,
		-1.f + dif / 2.f, -1.f + dif
	};
	gl::Buffer vbo("Instance Draw VBO");
	vbo.createStorage(static_cast<unsigned int>(tri.size()) * sizeof(float), 0, tri.data());

	// vao
	gl::VertexArray vao("Multi Draw VAO");
	glBindVertexArray(vao);
	vao.enableAttribBinding(0);
	vao.bindVertexBuffer(vbo, 0, 0, 2 * sizeof(float), 0);
	vao.bindVertexFormat(0, 2, GL_FLOAT, GL_FALSE, 0);

	prog.use();
	prog["col"] = glm::vec3{1.f, 0.f, 0.f};
	prog["width"] = NUM_TRIANGLES;
	while (e.render()) {
		timer.start();

		glDrawArraysInstanced(GL_TRIANGLES, 0, 3, primcount);

		timeDeque.emplace_back(timer.stop());
		if (timeDeque.size() == 100) {
			const auto ms = getAverageMs(timeDeque);
			timeDeque.erase(timeDeque.begin(), timeDeque.begin() + 50);
			LOG("Time: " + std::to_string(ms) + " ms");
		}
	}

}

#include "demo.hpp"

int main() {

	// const std::string title("monoEngine");
	const auto width = 1920u;
	const auto height = 1080u;
	// engine::Engine e(width, height, title, true);

	// // DEMO

	// core::Camera cam;
	// cam.setRatio(static_cast<float>(width) / static_cast<float>(height));
	// cam.setFov(glm::radians(45.f));
	// cam.translate({0.f, 0.f, 5.f});

	// // vertexPullingDemo();
	// // instancingElementDemo(e);
	// // instancingDemo(e);

	Demo demo(width, height);
	demo.use(Demo::RenderType::MULTIINDEX_CUBE);
	while (!demo.shouldClose()) {
		demo.render();
	}

	return EXIT_SUCCESS;

}
