#include <glm/gtx/rotate_vector.hpp>

#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 direction, float cameraSpeed):
	position_(position),
	direction_(direction),
	speed_(cameraSpeed),
	viewMatrix_(
		glm::lookAt(
			position,
			direction,
			glm::vec3(0,1,0)
		)
	)
{}

void Camera::updateView(){
	viewMatrix_ = glm::lookAt(
		position_,
		direction_,
		glm::vec3(0,1,0)
	);
}

void Camera::move(glm::vec2 direction){
	if(direction.x || direction.y){
		direction = glm::normalize(direction);
		float dz = direction.x;
		float dx = direction.y;

		// forward / backward
		position_ += speed_ * dx * direction_;
		// left / right
		position_ += speed_ * dz * glm::normalize( glm::cross(direction_, glm::vec3(0,1,0)) );
	}
}

void Camera::rotate(float pitch, float yaw){
	pitch_ += pitch;
	yaw_ += yaw;
	setRotation(pitch_, yaw_);
}

void Camera::setRotation(float theta, float phi){
	direction_.x = speed_ * cos(glm::radians(phi)) * cos(glm::radians(theta));
	direction_.y = speed_ * sin(glm::radians(phi));
	direction_.z = speed_ * cos(glm::radians(phi)) * sin(glm::radians(theta));
	direction_ = glm::normalize(direction_);
}

/**
 * Updates the view matrix uniform for the specified shader.
 * TODO use uniform block
 */
void Camera::uniformViewMatrix(Shader& shader) {
	glm::mat4 view = getViewMatrix();
	shader.setUniform("view", view);
}

void Camera::uniformPosition(Shader& shader) {
	shader.setUniform("cameraPos", position_);
}

glm::mat4 Camera::getViewMatrix() {
	updateView();
	return viewMatrix_;
}

