#pragma once

#include "../extern/glfwinclude.hpp"

#include <vector>
#include <functional>

namespace core {

class Input {

	public:

		Input(GLFWwindow * const window);
		Input(const Input & other) noexcept;

		void addKeyFunc(const std::function<void(int, int, int, int)> & func);
		void addCharFunc(const std::function<void(unsigned int)> & func);
		void addMouseCursorFunc(const std::function<void(double, double)> & func);
		void addMouseButtonFunc(const std::function<void(int, int, int)> & func);
		void addMouseScrollFunc(const std::function<void(double, double)> & func);

	private:

		void setCallbacks() const;

		GLFWwindow * const m_window;

		std::vector<std::function<void(int, int, int, int)>> m_keyFuncs;
		std::vector<std::function<void(unsigned int)>> m_charFuncs;
		std::vector<std::function<void(double, double)>> m_mouseCursorFuncs;
		std::vector<std::function<void(int, int, int)>> m_mouseButtonFuncs;
		std::vector<std::function<void(double, double)>> m_mouseScrollFuncs;

};

static_assert(std::is_nothrow_move_constructible<Input>(), "Should be noexcept MoveConstructible");
static_assert(std::is_nothrow_copy_constructible<Input>(), "Should be noexcept CopyConstructible");
static_assert(!std::is_move_assignable<Input>(), "Should not be MoveAssignable");
static_assert(!std::is_copy_assignable<Input>(), "Should not be CopyAssignable");

} // namespace core
