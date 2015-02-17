#include "timer.hpp"

#include "../core/log.hpp"

namespace gl {

Timer::Timer()
  : m_front{GL_TIME_ELAPSED},
	m_back{GL_TIME_ELAPSED},
	m_swapped{false}
{
	if (m_front == 0 || m_back == 0) {
		LOG("Error creating Timer!");
	}
}

Timer::Timer(const std::string & name)
   : m_front{GL_TIME_ELAPSED},
	m_back{GL_TIME_ELAPSED},
	m_swapped{false}
{
	if (m_front == 0 || m_back == 0) {
		LOG("Error creating Timer!");
	} else {
		glObjectLabel(GL_QUERY, m_front, static_cast<GLsizei>(name.size()), name.c_str());
		glObjectLabel(GL_QUERY, m_back, static_cast<GLsizei>(name.size()), name.c_str());
	}
}

void Timer::start() const {
	glBeginQuery(GL_TIME_ELAPSED, m_swapped ? m_front : m_back);
}

GLuint64 Timer::stop() const {
	glEndQuery(GL_TIME_ELAPSED);
	GLuint64 t;
	glGetQueryObjectui64v(m_swapped ? m_back : m_back, GL_QUERY_RESULT, &t);
	m_swapped = !m_swapped;
	return t;
}

} // namespace gl
