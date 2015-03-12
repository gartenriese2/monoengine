#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace core {

Camera::Camera()
  : m_pos{0.f, 0.f, 0.f},
	m_dir{0.f, 0.f, -1.f},
	m_up{0.f, 1.f, 0.f},
	m_fov{45.f},
	m_ratio{1.f},
	m_near{0.01f},
	m_far{std::numeric_limits<float>::infinity()},
	m_modified{true}
{
	m_projMat = glm::infinitePerspective(m_fov, m_ratio, m_near);
}

Camera::Camera(const glm::vec3 & pos, const glm::vec3 & dir, const glm::vec3 & up,
		const float fov, const float ratio, const float near, const float far)
  : m_pos{pos},
	m_dir{glm::normalize(dir)},
	m_up{glm::normalize(up)},
	m_fov{fov},
	m_ratio{ratio},
	m_near{near},
	m_far{far},
	m_modified{true}
{
	LOG_ASSERT(std::abs(dir.length()) > 0.f, "Camera: length of dir > 0");
	LOG_ASSERT(std::abs(up.length()) > 0.f, "Camera: length of up > 0");
	LOG_ASSERT(near > 0.f, "Camera: near plane is not positive");
	LOG_ASSERT(far > near, "Camera: far plane is not greater than near plane");
	if (m_far >= std::numeric_limits<float>::infinity()) {
		m_projMat = glm::infinitePerspective(m_fov, m_ratio, m_near);
	} else {
		m_projMat = glm::perspective(m_fov, m_ratio, m_near, m_far);
	}
}

const glm::mat4 & Camera::getViewMatrix() const {

	if (m_modified) {
		// Compute inverse rotation q
		auto q = m_orientation;
		q.x *= -1.0f;
		q.y *= -1.0f;
		q.z *= -1.0f;
		m_viewMat = glm::mat4_cast(q);

		// Translate by inverse eyePosition.
		const auto v = -m_pos;
		const auto m = m_viewMat;
		m_viewMat[3] = (m[0] * v[0]) + (m[1] * v[1]) + (m[2] * v[2]) + m[3];

		m_modified = false;
	}

	return m_viewMat;

}

const glm::mat4 & Camera::getProjMatrix() const {

	if (m_modifiedProj) {
		if (m_far >= std::numeric_limits<float>::infinity()) {
			m_projMat = glm::infinitePerspective(m_fov, m_ratio, m_near);
		} else {
			m_projMat = glm::perspective(m_fov, m_ratio, m_near, m_far);
		}

		m_modifiedProj = false;
	}

	return m_projMat;

}

void Camera::setFov(const float val) {
	m_fov = val;
	m_modifiedProj = true;
}

void Camera::setRatio(const float val) {
	m_ratio = val;
	m_modifiedProj = true;
}

void Camera::setNear(const float val) {
	m_near = val;
	m_modifiedProj = true;
}

void Camera::setFar(const float val) {
	m_far = val;
	m_modifiedProj = true;
}

void Camera::roll(const float angle) {

	if (std::abs(angle) <= 0.f) {
		return;
	}

	const glm::quat q = glm::angleAxis(angle, m_dir);

	m_up = glm::normalize(glm::rotate(q, m_up));

	m_orientation = glm::normalize(q * m_orientation);

	m_modified = true;

}

void Camera::pitch(const float angle) {

	if (std::abs(angle) <= 0.f) {
		return;
	}

	const glm::quat q = glm::angleAxis(angle, glm::cross(m_dir, m_up));

	m_up = glm::normalize(glm::rotate(q, m_up));
	m_dir = glm::normalize(glm::rotate(q, m_dir));

	m_orientation = glm::normalize(q * m_orientation);

	m_modified = true;

}

void Camera::yaw(const float angle) {

	if (std::abs(angle) <= 0.f) {
		return;
	}

	const glm::quat q = glm::angleAxis(angle, m_up);

	m_dir = glm::normalize(glm::rotate(q, m_dir));

	m_orientation = glm::normalize(q * m_orientation);

	m_modified = true;

}

void Camera::rotate(const float angle, const glm::vec3 & axis) {

	if (std::abs(angle) <= 0.f) {
		return;
	}

	LOG_ASSERT(std::abs(axis.length()) > 0.f, "rotation axis > 0.f");

	const auto n = glm::normalize(axis);
	const auto q = glm::angleAxis(angle, n);

	m_dir = glm::normalize(glm::rotate(q, m_dir));
	m_up = glm::normalize(glm::rotate(q, m_up));

	m_orientation = glm::normalize(q * m_orientation);

	m_modified = true;

}

void Camera::translate(const glm::vec3 & v) {

	if (std::abs(static_cast<float>(v.length())) <= 0.f) {
		return;
	}

	m_pos += v;

	m_modified = true;

}

void Camera::translateLocal(const glm::vec3 & v) {

	if (std::abs(static_cast<float>(v.length())) <= 0.f) {
		return;
	}

	m_pos += v.x * glm::cross(m_dir, m_up);
	m_pos += v.y * m_up;
	m_pos -= v.z * m_dir;

	m_modified = true;

}

} // namespace core
