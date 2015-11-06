#include <glm/gtc/matrix_transform.hpp>

#include "Screen.hpp"

Screen::Screen(float fovy, int width, int height, float zNear, float zFar) {
	this->fovy_ = fovy;
	this->width_ = width;
	this->height_ = height;
	this->zNear_ = zNear;
	this->zFar_ = zFar;
}

void Screen::reshape(int width, int height) {
	this->width_ = width;
	this->height_ = height;

	glViewport(0, 0, width_, height_);
}

void Screen::uniformProjectionMatrix(Shader& shader) {
	glm::mat4 projectionMatrix = glm::perspective(
		fovy_, (float) width_/height_, zNear_, zFar_
	);
	shader.setUniform("projection", projectionMatrix);
}

int Screen::getWidth() {
	return width_;
}

int Screen::getHeight() {
	return height_;
}
