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
				MULTI_TRIANGLE, MULTI_CUBE,
				MULTIINDEX_TRIANGLE, MULTIINDEX_CUBE,
				INSTANCE_TRIANGLE, INSTANCE_CUBE,
				INSTANCEINDEX_TRIANGLE, INSTANCEINDEX_CUBE
		};

		Demo(const glm::uvec2 &);

		bool render();

		void use(RenderType);

	private:

		void initShaders();
		void initBuffers();
		void initVAOs();
		void initFunctions();
		double getAverageMs(const std::deque<GLuint64> &);

		engine::Engine m_engine;
		core::Camera m_cam;

		gl::Program m_basicProg;
		gl::Program m_instanceProg;

		gl::Buffer m_vboTriangle;
		gl::Buffer m_vboCube;
		gl::Buffer m_vboCubeIndexed;

		gl::Buffer m_iboTriangle;
		gl::Buffer m_iboCube;
		gl::Buffer m_iboMultiTriangle;
		gl::Buffer m_iboMultiCube;
		std::vector<GLint> m_multiOffsetsTriangle;
		std::vector<GLsizei> m_multiCountsTriangle;
		std::vector<GLint> m_multiOffsetsCube;
		std::vector<GLsizei> m_multiCountsCube;
		std::vector<GLchar *> m_multiOffsetsTriangleIndexed;
		std::vector<GLsizei> m_multiCountsTriangleIndexed;
		std::vector<GLint> m_multiBaseVertexTriangle;
		std::vector<GLchar *> m_multiOffsetsCubeIndexed;
		std::vector<GLsizei> m_multiCountsCubeIndexed;
		std::vector<GLint> m_multiBaseVertexCube;

		gl::VertexArray m_vaoTriangle;
		gl::VertexArray m_vaoTriangleIndex;
		gl::VertexArray m_vaoTriangleIndexMulti;
		gl::VertexArray m_vaoCube;
		gl::VertexArray m_vaoCubeIndex;
		gl::VertexArray m_vaoCubeIndexMulti;

		gl::Timer m_timer;
		std::deque<GLuint64> m_timeDeque;

		std::map<RenderType, std::function<void()>> m_funcs;
		std::function<void()> m_renderFunc;

};
