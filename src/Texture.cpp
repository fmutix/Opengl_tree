#include <SOIL/SOIL.h>
#include <GL/glew.h>

#include "Texture.hpp"

Texture::Texture() {}

Texture::Texture(std::string name) {
	image_ = SOIL_load_image(name.c_str(), &width_, &height_, 0, SOIL_LOAD_RGB);
	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, width_, height_,
		0, GL_RGB, GL_UNSIGNED_BYTE, image_
	);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image_);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getTexture() {
	return texture_;
}
