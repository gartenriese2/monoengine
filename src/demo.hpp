#pragma once

#include "engine/engine.hpp"
#include "engine/core/camera.hpp"
#include "engine/gl/program.hpp"
#include "engine/gl/buffer.hpp"
#include "engine/gl/vertexarray.hpp"
#include "engine/gl/timer.hpp"
#include "engine/object.hpp"

#include <deque>
#include <vector>

class Demo {

	public:

		Demo(const glm::uvec2 &);

		bool render();

	private:

		void init();
		void orderModels();
		void setModelMatrices();
		double getAverageMs(const std::deque<GLuint64> &);
		double getAverageMs(const std::deque<double> &);

		engine::Engine m_engine;
		core::Camera m_cam;

		gl::Program m_prog;

		gl::Buffer m_vbo;
		gl::Buffer m_ibo;
		gl::Buffer m_modelMatrixBuffer;
		std::vector<engine::Object> m_objects;

		gl::VertexArray m_vao;

		gl::Timer m_timer;
		std::deque<GLuint64> m_timeDeque;
		std::deque<double> m_cpuTimeDeque;

		unsigned int m_numObjects;

};
