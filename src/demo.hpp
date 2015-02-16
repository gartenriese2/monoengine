#pragma once

#include "engine/engine.hpp"
#include "engine/core/camera.hpp"
#include "engine/gl/program.hpp"
#include "engine/gl/buffer.hpp"
#include "engine/gl/vertexarray.hpp"
#include "engine/gl/timer.hpp"

#include <map>
#include <vector>
#include <deque>

class Demo {

	public:

		enum class RenderType : std::int8_t {
				BASIC_TRIANGLE, BASIC_CUBE,
				BASICINDEX_TRIANGLE, BASICINDEX_CUBE,
				MULTI_TRIANGLE, MULTI_CUBE
		};

		Demo(unsigned int, unsigned int);

		void render();

		void use(RenderType);

		bool shouldClose() const { return m_shouldClose; }

	private:

		void initShaders();
		void initBuffers();
		void initVAOs();
		void initFunctions();
		double getAverageMs(const std::deque<GLuint64> &);

		engine::Engine m_engine;
		core::Camera m_cam;

		gl::Program m_basicProg;

		gl::Buffer m_vboTriangle;
		gl::Buffer m_vboCube;
		gl::Buffer m_vboCubeIndexed;
		gl::Buffer m_iboTriangle;
		gl::Buffer m_iboCube;
		std::vector<GLint> m_multiOffsets;
		std::vector<GLsizei> m_multiCounts;
		std::vector<GLint> m_multiOffsetsCube;
		std::vector<GLsizei> m_multiCountsCube;

		gl::VertexArray m_vaoTriangle;
		gl::VertexArray m_vaoTriangleIndex;
		gl::VertexArray m_vaoCube;
		gl::VertexArray m_vaoCubeIndex;

		gl::Timer m_timer;
		std::deque<GLuint64> m_timeDeque;

		std::map<RenderType, std::function<void()>> m_funcs;
		std::function<void()> m_renderFunc;

		bool m_shouldClose;

};
