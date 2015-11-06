#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

#include "Object3D.hpp"

Object3D::Object3D() {}

Object3D::Object3D(std::string filename) {
	init(filename);
}

void Object3D::init(std::string filename) {
	readMesh(filename);
	
	if (normals_.empty()) {
		computeNormals();
	}

	createObjectBuffers();
}

void Object3D::readMesh(std::string filename) {
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
		else if (elementType == "vn") {
			glm::vec3 vn;
			ss >> vn.x;
			ss >> vn.y;
			ss >> vn.z;
			glm::normalize(vn);
			normals_.push_back(vn);
		}
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

void Object3D::display() {
	// Bind VAO & index buffer
	glBindVertexArray(vao_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
	glBindTexture(GL_TEXTURE_2D, texture_.getTexture());

	// Display
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);

	// Unbind VAO & index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
