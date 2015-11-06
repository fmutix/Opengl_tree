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

glm::vec3 Camera::rotate(float vx, float vy) {
	float angle = 4.0f * (fabs(vx) + fabs(vy));
	glm::vec3 directionToMoveIn = (-getRight() * vx) +
								  (-up_ * vy);
	glm::vec3 rotationAxis = glm::cross(directionToMoveIn, forward_);

	rotate(angle, rotationAxis);

	return rotationAxis;
}

void Camera::rotate(float angle, glm::vec3 rotationAxis) {
	position_ = glm::rotate(position_, angle, rotationAxis);
	up_ = glm::normalize(glm::rotate(up_, angle, rotationAxis));
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
	return glm::lookAt(
		position_,
		position_ + forward_,
		up_
	);
}

glm::vec3 Camera::getPosition() {
	return position_;
}

glm::vec3 Camera::getUp() {
	return up_;
}

glm::vec3 Camera::getForward() {
	return forward_;
}

glm::vec3 Camera::getRight() {
	return glm::normalize(glm::cross(forward_, up_));
}
