#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/vec3.hpp>

#include "Shader.hpp"

class Camera {
public:
	Camera(
		const glm::vec3& position, const glm::vec3& up,
		const glm::vec3& forward
	);

	void rotate(float angle, glm::vec3 rotationAxis);
	void moveForwardBackward(float value);
	void moveLeftRight(float value);
	void reset();

	void uniformViewMatrix(Shader& shader);
	void uniformPosition(Shader& shader);

	glm::mat4 getViewMatrix();

private:
	glm::vec3 position_;
	glm::vec3 up_;
	glm::vec3 forward_;

	glm::vec3 initPosition_;
	glm::vec3 initUp_;
	glm::vec3 initForward_;

	glm::vec3 right_;
};

#endif // OBJECT3D_HPP
