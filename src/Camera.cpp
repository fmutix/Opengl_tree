#include <glm/gtx/rotate_vector.hpp>

#include "Camera.hpp"

Camera::Camera(
	const glm::vec3& position, const glm::vec3& up,
	const glm::vec3& forward) {
	this->position_ = position;
	this->up_ = up;
	this->forward_ = forward;

	initPosition_ = position;
	initUp_ = up;
	initForward_ = forward;
}

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

void Camera::rotate(float angle, glm::vec3 rotationAxis) {
	position_ = glm::rotate(position_, angle, rotationAxis);
	forward_ = glm::normalize(glm::rotate(forward_, angle, rotationAxis));
}

void Camera::moveForwardBackward(float value) {
	position_ += forward_ * value;
}

void Camera::moveLeftRight(float value) {
	forward_ = glm::normalize(glm::rotate(forward_, value, up_));
}

/**
 * Resets the camera's vectors to the values they were initialized to.
 */
void Camera::reset() {
	position_ = initPosition_;
	up_ = initUp_;
	forward_ = initForward_;
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

