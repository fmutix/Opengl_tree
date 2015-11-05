#include <glm/gtx/rotate_vector.hpp>

#include "Camera.hpp"

Camera::Camera(
	const glm::vec3& position, const glm::vec3& up,
	const glm::vec3& forward) {
	this->position = position;
	this->up = up;
	this->forward = forward;

	initPosition = position;
	initUp = up;
	initForward = forward;
}

glm::vec3 Camera::rotate(float vx, float vy) {
	float angle = 4.0f * (fabs(vx) + fabs(vy));
	glm::vec3 directionToMoveIn = (-getRight() * vx) +
								  (-up * vy);
	glm::vec3 rotationAxis = glm::cross(directionToMoveIn, forward);

	rotate(angle, rotationAxis);

	return rotationAxis;
}

void Camera::rotate(float angle, glm::vec3 rotationAxis) {
	position = glm::rotate(position, angle, rotationAxis);
	up = glm::normalize(glm::rotate(up, angle, rotationAxis));
	forward = glm::normalize(glm::rotate(forward, angle, rotationAxis));
}

void Camera::moveForwardBackward(float value) {
	position += forward * value;
}

void Camera::moveLeftRight(float value) {
	forward = glm::normalize(glm::rotate(forward, value, up));
}

/**
 * Resets the camera's vectors to the values they were initialized to.
 */
void Camera::reset() {
	position = initPosition;
	up = initUp;
	forward = initForward;
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
	shader.setUniform("cameraPos", position);
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(
		position,
		position + forward,
		up
	);
}

glm::vec3 Camera::getPosition() {
	return position;
}

glm::vec3 Camera::getUp() {
	return up;
}

glm::vec3 Camera::getForward() {
	return forward;
}

glm::vec3 Camera::getRight() {
	return glm::normalize(glm::cross(forward, up));
}
