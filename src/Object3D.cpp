#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

#include "Object3D.hpp"
#include <openctmpp.h>

Object3D::Object3D() {}

Object3D::Object3D(std::string filename, glm::vec3 position, std::string tex) {
	color_ = glm::vec3(1.0f, 1.0f, 1.0f);
	position_ = position;
	scale_ = 1.0;
	init(filename);
	texture_ = Texture(tex);
	textured_ = true;
}

Object3D::Object3D(std::string filename, glm::vec3 position, glm::vec3 color) {
	color_ = color;
	position_ = position;
	scale_ = 1.0;
	init(filename);
	textured_ = false;
}

void Object3D::init(std::string filename) {
	readMesh(filename);

	if (normals_.empty()) {
		computeNormals();
	}

	createObjectBuffers();
}

void Object3D::readMesh(std::string filename) {
	try{
		// Create a new OpenCTM importer object
		CTMimporter ctm;
		// Load the OpenCTM file
		ctm.Load(filename.c_str());
		// Access the mesh data

		CTMuint vertCount = ctm.GetInteger(CTM_VERTEX_COUNT);
		const CTMfloat* vertices = ctm.GetFloatArray(CTM_VERTICES);
		CTMuint triCount = ctm.GetInteger(CTM_TRIANGLE_COUNT);
		const CTMuint* indices = ctm.GetIntegerArray(CTM_INDICES);

		for(unsigned int i = 0; i < 3 * vertCount; i+=3){
			glm::vec3 v;
			v.x = vertices[i];
			v.y = vertices[i+1];
			v.z = vertices[i+2];
			vertices_.push_back(v);
		}
		for(unsigned int i = 0; i < 3 * triCount; i+=3){
			indices_.push_back(indices[i]);
			indices_.push_back(indices[i+1]);
			indices_.push_back(indices[i+2]);
		}

		// Deal with the mesh (e.g. transcode it to our
		// internal representation)
		// ...
	}catch(std::exception &e){
		std::cout << "Error:" << e.what() << std::endl;
		exit(1);
	}
}

glm::vec3 Object3D::computeTriangleNormal(unsigned int faceId) {
	glm::uvec3 face = getFace(faceId);

	return computeTriangleNormal(
		vertices_[face[0]], vertices_[face[1]], vertices_[face[2]]
	);
}

glm::vec3 Object3D::computeTriangleNormal(
	const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3
) {
	return glm::normalize(
		glm::cross(v2 - v1, v3 - v1)
	);
}

void Object3D::normalizeNormals() {
	for (glm::vec3& normal : normals_) {
		normal = glm::normalize(normal);
	}
}

void Object3D::computeNormals() {
	normals_.resize(3 * facesSize());
	for (unsigned int faceId = 0; faceId < facesSize(); faceId++) {
		glm::vec3 faceNormal = computeTriangleNormal(faceId);
		// Contribution from the current's face normal to the adjacent vertices
		// normals.
		glm::uvec3 face = getFace(faceId);
		normals_[face[0]] += faceNormal;
		normals_[face[1]] += faceNormal;
		normals_[face[2]] += faceNormal;
	}

	normalizeNormals();
}


void Object3D::createObjectBuffers() {
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

	// Normal buffer
	glGenBuffers(1, &normalBuffer_);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer_);
	glBufferData(
		GL_ARRAY_BUFFER, normals_.size() * sizeof(glm::vec3),
		&normals_.front(), GL_STATIC_DRAW
	);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(1);

	// Unbind normal buffer
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

GLuint Object3D::getTexture() {
	return texture_.getTexture();
}

void Object3D::setTexture(Texture& texture) {
	this->texture_ = texture;
}

glm::vec3 Object3D::getColor() {
	return color_;
}

glm::vec3 Object3D::getPosition() {
	return position_;
}

float Object3D::getScale() {
	return scale_;
}

void Object3D::setScale(float scale) {
	scale_ = scale;
}

bool Object3D::hasTexture() {
	return textured_;
}

void Object3D::display() {
	// Bind VAO & index buffer
	glBindVertexArray(vao_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
	if (hasTexture()) {
		glBindTexture(GL_TEXTURE_2D, texture_.getTexture());
	}

	// Display
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);

	// Unbind VAO & index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	if (hasTexture()) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

glm::uvec3 Object3D::getFace(unsigned int faceId) {
	return glm::uvec3(
		indices_[faceId * 3],
		indices_[(faceId * 3) + 1],
		indices_[(faceId * 3) + 2]
	);
}

unsigned int Object3D::facesSize() {
	return indices_.size() / 3;
}
