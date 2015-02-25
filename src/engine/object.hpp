#pragma once

#include <glm/glm.hpp>

namespace engine {

class Object {

	public:

		Object();

		const auto & getModelMatrix() const { return m_model; }

		void move(float, const glm::vec3 &);
		void moveLocal(float, const glm::vec3 &);
		void moveTo(const glm::vec3 &);
		void scale(const glm::vec3 &);

	private:

		glm::mat4 m_model;

		glm::mat4 m_scale;
		glm::mat4 m_rotate;
		glm::mat4 m_translate;

		glm::vec3 m_originalPosition;
		glm::vec3 m_actualPosition;

};

} // namespace engine
