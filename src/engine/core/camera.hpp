#pragma once

#include <glm/gtc/quaternion.hpp>

#include <limits>

namespace core {

class Camera {

	public:

		Camera();
		Camera(const glm::vec3 &, const glm::vec3 &, const glm::vec3 &,
				float, float, float, float = std::numeric_limits<float>::infinity());

		const glm::mat4 & getViewMatrix() const;
		const glm::mat4 & getProjMatrix() const;

		void setFov(float);
		void setRatio(float);
		void setNear(float);
		void setFar(float);

		void roll(float); // rotate around z-axis
		void pitch(float); // rotate around x-axis
		void yaw(float); // rotate around y-axis
		void rotate(float, const glm::vec3 &);
		void translate(const glm::vec3 &);
		void translateLocal(const glm::vec3 &);

	private:

		glm::vec3 m_pos;
		glm::vec3 m_dir;
		glm::vec3 m_up;
		glm::quat m_orientation;

		float m_fov;
		float m_ratio;
		float m_near;
		float m_far;

		mutable glm::mat4 m_viewMat;
		mutable glm::mat4 m_projMat;

		mutable bool m_modified;
		mutable bool m_modifiedProj;

};

} // namespace core
