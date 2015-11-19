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

	Camera(glm::vec3 position, glm::vec3 direction, float cameraSpeed = 0.1);

	void updateView();
	void move(glm::vec2 direction);
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


	glm::vec3 direction_;
	float speed_;
	glm::mat4 viewMatrix_;
};

#endif // OBJECT3D_HPP
