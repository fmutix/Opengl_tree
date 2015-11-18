#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

#include "Skybox.hpp"

Skybox::Skybox() {}

Skybox::Skybox(glm::vec3 position) {
	position_ = position;
	scale_ = 20.0;
	cubeTextures_.push_back("res/textures/right.jpg");
	cubeTextures_.push_back("res/textures/left.jpg");
	cubeTextures_.push_back("res/textures/top.jpg");
	cubeTextures_.push_back("res/textures/bottom.jpg");
	cubeTextures_.push_back("res/textures/back.jpg");
	cubeTextures_.push_back("res/textures/front.jpg");
	init("res/obj/cube.ctm");
}

void Skybox::init(std::string filename) {
	readMesh(filename);
	initTexture();
	createObjectBuffers();
}

void Skybox::readMesh(std::string filename) {
	std::ifstream ifs(filename.c_str(), std::ios::in);

	if (!ifs) {
		std::cerr << filename << " does not exist." << std::endl;
		exit(1);
	}

	std::string line;
	while (getline(ifs, line)) {
		std::stringstream ss(line);

		std::string elementType;
		ss >> elementType;

		if (elementType == "v") {
			glm::vec3 v;
			ss >> v.x;
			ss >> v.y;
			ss >> v.z;
			glm::normalize(v);
			vertices_.push_back(v);
		}
		else if (elementType == "f") {
			GLuint value;
			while(ss >> value) {
				value--;
				indices_.push_back(value);
			}
		}
	}
}

void Skybox::createObjectBuffers() {
	// Generate and bind VAO
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// Vertex buffer
	glGenBuffers(1, &vertexBuffer_);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
	glBufferData(
		GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3),
		&vertices_.front(), GL_STATIC_DRAW
	);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(0);

	// Unbind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO
	glBindVertexArray(0);

	// Generate and bind index buffer
	glGenBuffers(1, &indexBuffer_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint),
		&indices_.front(), GL_STATIC_DRAW
	);

	// Unbind index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Skybox::initTexture() {
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);

	for (unsigned int i = 0; i < cubeTextures_.size(); i++) {
		image_ = SOIL_load_image(
			cubeTextures_[i].c_str(), &width_, &height_, 0, SOIL_LOAD_RGB
		);
		if (image_ == NULL) {
			std::cerr << "Error loading skybox tex with SOIL" << std::endl;
		}
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
			width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, image_
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	SOIL_free_image_data(image_);
}

GLuint Skybox::getTexture() {
	return texture_;
}

float Skybox::getScale() {
	return scale_;
}

void Skybox::setScale(float scale) {
	scale_ = scale;
}

void Skybox::setUniform(Shader& shader) {
	shader.setUniform("objectPos", position_);
	shader.setUniform("objectScale", scale_);
}

void Skybox::display() {
	// Bind VAO & index buffer
	glBindVertexArray(vao_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);

	// Display
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);

	// Unbind VAO & index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::uvec3 Skybox::getFace(unsigned int faceId) {
	return glm::uvec3(
		indices_[faceId * 3],
		indices_[(faceId * 3) + 1],
		indices_[(faceId * 3) + 2]
	);
}

unsigned int Skybox::facesSize() {
	return indices_.size() / 3;
}
