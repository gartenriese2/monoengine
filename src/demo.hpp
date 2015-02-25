#pragma once

#include "engine/engine.hpp"
#include "engine/core/camera.hpp"
#include "engine/gl/program.hpp"
#include "engine/gl/buffer.hpp"
#include "engine/gl/vertexarray.hpp"
#include "engine/gl/timer.hpp"

#include <deque>

class Demo {

	public:

		Demo(const glm::uvec2 &);

		bool render();

	private:

		void init();
		void setModelMatrices();
		double getAverageMs(const std::deque<GLuint64> &);

		engine::Engine m_engine;
		core::Camera m_cam;

		gl::Program m_prog;

		gl::Buffer m_vbo;
		gl::Buffer m_ibo;
		gl::Buffer m_modelMatrixBuffer;

		gl::VertexArray m_vao;

		gl::Timer m_timer;
		std::deque<GLuint64> m_timeDeque;

		unsigned int m_numObjects;

};
