#pragma once

#include "globject.hpp"

#include <string>

namespace gl {

class Timer {

	public:

		Timer();
		Timer(const std::string & name);
		Timer(const Timer &) = delete;
		Timer(Timer &&) = default;
		Timer & operator=(const Timer &) = delete;
		Timer & operator=(Timer &&) & = default;
		~Timer() {}

		void start() const;
		GLuint64 stop() const;

	private:

		GLObject m_front;
		GLObject m_back;

		mutable bool m_swapped;

};

static_assert(std::is_nothrow_move_constructible<Timer>(), "Should be noexcept MoveConstructible");
static_assert(!std::is_copy_constructible<Timer>(), "Should not be CopyConstructible");
static_assert(std::is_nothrow_move_assignable<Timer>(), "Should be noexcept MoveAssignable");
static_assert(!std::is_copy_assignable<Timer>(), "Should not be CopyAssignable");

} // namespace gl
