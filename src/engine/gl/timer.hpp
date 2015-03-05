#pragma once

#include "globject.hpp"

#include <string>

namespace gl {

class Timer {

	public:

		Timer();
		Timer(const Timer &) = delete;
		Timer(Timer &&) = default;
		Timer & operator=(const Timer &) = delete;
		Timer & operator=(Timer &&) = default;
		~Timer() {}

		void start() const;
		GLuint64 stop() const;

	private:

		GLObject m_front;
		GLObject m_back;

		mutable bool m_swapped;

};

} // namespace gl
