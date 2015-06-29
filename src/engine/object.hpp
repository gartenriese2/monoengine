#pragma once

#include <glm/glm.hpp>

namespace engine {

class Object {

	public:

		Object();

		const auto & getModelMatrix() const noexcept { return m_model; }
		const auto & getScaleMatrix() const noexcept { return m_scale; }

		void move(float val, const glm::vec3 & dir);
		void moveLocal(float val, const glm::vec3 & dir);
		void moveTo(const glm::vec3 & to);
		void resetMoves();

		void scale(const glm::vec3 & val);
		void resetScale();

		void rotate(float radians, const glm::vec3 & axis);
		void rotateAround(float radians, const glm::vec3 & axis, const glm::vec3 & point = {0.f, 0.f, 0.f});

	private:

		glm::mat4 m_model;

		glm::mat4 m_scale;
		glm::mat4 m_rotate;
		glm::mat4 m_translate;

		glm::vec3 m_actualPosition;

};

static_assert(std::is_nothrow_move_constructible<Object>(), "Should be noexcept MoveConstructible");
static_assert(std::is_nothrow_copy_constructible<Object>(), "Should be noexcept CopyConstructible");
static_assert(std::is_nothrow_move_assignable<Object>(), "Should be noexcept MoveAssignable");
static_assert(std::is_nothrow_copy_assignable<Object>(), "Should be noexcept CopyAssignable");

} // namespace engine
