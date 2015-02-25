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

void Object::move(float val, const glm::vec3 & dir) {
	if (glm::length(dir) != 0.f) {
		m_translate = glm::translate(m_translate, val * dir);
		m_model = m_translate * m_rotate * m_scale;
		m_actualPosition += (val * dir);
	}
}

void Object::moveLocal(float val, const glm::vec3 & dir) {
	if (glm::length(dir) != 0.f) {
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

void Object::scale(const glm::vec3 & val) {
	if (glm::length(val) != 0.f) {
		m_scale = glm::translate(m_scale, m_originalPosition);
		m_scale = glm::scale(m_scale, val);
		m_scale = glm::translate(m_scale, -m_originalPosition);
		m_model = m_translate * m_rotate * m_scale;
	}
}

} // namespace engine
