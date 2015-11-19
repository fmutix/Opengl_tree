#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/vec3.hpp>

#include "Shader.hpp"

class Camera {
public:
	Camera(glm::vec3 position, glm::vec3 direction, float cameraSpeed = 0.1);

	void updateView();
	void move(glm::vec2 direction);
	void rotate(float pitch, float yaw);
	void setRotation(float theta, float phi);

	void uniformViewMatrix(Shader& shader);
	void uniformPosition(Shader& shader);

	glm::mat4 getViewMatrix();

private:
	glm::vec3 position_;
	glm::vec3 direction_;
	float speed_;
	glm::mat4 viewMatrix_;
	float pitch_ = -90;
	float yaw_ = 0;

	glm::vec3 initPosition_;
	glm::vec3 initUp_;
	glm::vec3 initForward_;
};

#endif // OBJECT3D_HPP
