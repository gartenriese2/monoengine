#include "object.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

Object::Object()
  : m_model{1.f},
	m_scale{1.f},
	m_rotate{1.f},
	m_translate{1.f}
{

}

void Object::move(const float val, const glm::vec3 & dir) {
	if (std::abs(glm::length(dir)) > 0.f) {
		m_translate = glm::translate(m_translate, val * dir);
		m_model = m_translate * m_rotate * m_scale;
		m_actualPosition += (val * dir);
	}
}

void Object::moveLocal(const float val, const glm::vec3 & dir) {
	if (std::abs(glm::length(dir)) > 0.f) {
		m_translate = glm::translate(m_translate * m_rotate, val * dir);
		m_model = m_translate * m_rotate * m_scale;
		m_actualPosition += (val * dir);
	}
}

void Object::moveTo(const glm::vec3 & to) {
	m_translate = glm::translate(m_translate, to - m_actualPosition);
	m_model = m_translate * m_rotate * m_scale;
	m_actualPosition = to;
}

void Object::resetMoves() {
	m_translate = glm::mat4{1.f};
	m_actualPosition = glm::vec3{0.f};
}

void Object::scale(const glm::vec3 & val) {
	if (std::abs(glm::length(val)) > 0.f) {
		m_scale = glm::scale(m_scale, val);
		m_model = m_translate * m_rotate * m_scale;
	}
}

void Object::resetScale() {
	m_scale = glm::mat4{1.f};
}

void Object::rotate(const float radians, const glm::vec3 & axis) {
	if (std::abs(glm::length(axis)) > 0.f) {
		const auto norm = glm::normalize(axis);
		m_rotate = glm::rotate(m_rotate, radians, norm);
		m_model = m_translate * m_rotate * m_scale;
	}
}

void Object::rotateAround(const float radians, const glm::vec3 & axis, const glm::vec3 & point) {
	if (std::abs(glm::length(axis)) > 0.f) {
		rotate(radians, axis);

		// http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/ArbitraryAxisRotation.html -> Section 6.2
		const auto x = m_actualPosition.x;
		const auto y = m_actualPosition.y;
		const auto z = m_actualPosition.z;
		const auto a = point.x;
		const auto b = point.y;
		const auto c = point.z;
		const auto nAxis = glm::normalize(axis);
		const auto u = nAxis.x;
		const auto v = nAxis.y;
		const auto w = nAxis.z;

		const auto retX = (a * (v*v + w*w) - u * (b*v + c*w - u*x - v*y - w*z)) * (1 - glm::cos(radians))
			+ x * glm::cos(radians) + (-c*v + b*w - w*y + v*z) * glm::sin(radians);
		const auto retY = (b * (u*u + w*w) - v * (a*u + c*w - u*x - v*y - w*z)) * (1 - glm::cos(radians))
			+ y * glm::cos(radians) + (c*u - a*w + w*x - u*z) * glm::sin(radians);
		const auto retZ = (c * (u*u + v*v) - w * (a*u + b*v - u*x - v*y - w*z)) * (1 - glm::cos(radians))
			+ z * glm::cos(radians) + (-b*u + a*v - v*x + u*y) * glm::sin(radians);

		moveTo({retX, retY, retZ});
	}
}

} // namespace engine
