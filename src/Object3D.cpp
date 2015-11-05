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
	
	if (normals.empty()) {
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
			vertices.push_back(v);
		}
		else if (elementType == "f") {
			GLuint value;
			while(ss >> value) {
				value--;
				indices.push_back(value);
			}
		}
		else if (elementType == "vn") {
			glm::vec3 vn;
			ss >> vn.x;
			ss >> vn.y;
			ss >> vn.z;
			glm::normalize(vn);
			normals.push_back(vn);
		}
	}
}

glm::vec3 Object3D::computeTriangleNormal(unsigned int faceId) {
	glm::uvec3 face = getFace(faceId);

	return computeTriangleNormal(
		vertices[face[0]], vertices[face[1]], vertices[face[2]]
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
	for (glm::vec3& normal : normals) {
		normal = glm::normalize(normal);
	}
}

void Object3D::computeNormals() {
	normals.resize(3 * facesSize());
	for (unsigned int faceId = 0; faceId < facesSize(); faceId++) {
		glm::vec3 faceNormal = computeTriangleNormal(faceId);
		// Contribution from the current's face normal to the adjacent vertices
		// normals.
		glm::uvec3 face = getFace(faceId);
		normals[face[0]] += faceNormal;
		normals[face[1]] += faceNormal;
		normals[face[2]] += faceNormal;
	}
	
	normalizeNormals();
}


void Object3D::createObjectBuffers() {
	// Generate and bind VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Vertex buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
		&vertices.front(), GL_STATIC_DRAW
	);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(0);

	// Unbind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Normal buffer
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(
		GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
		&normals.front(), GL_STATIC_DRAW
	);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(1);

	// Unbind normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO
	glBindVertexArray(0);

	// Generate and bind index buffer
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
		&indices.front(), GL_STATIC_DRAW
	);

	// Unbind index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint Object3D::getTexture() {
	return texture.getTexture();
}

void Object3D::setTexture(Texture& texture) {
	this->texture = texture;
}

void Object3D::display() {
	// Bind VAO & index buffer
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBindTexture(GL_TEXTURE_2D, texture.getTexture());

	// Display
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind VAO & index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

glm::uvec3 Object3D::getFace(unsigned int faceId) {
	return glm::uvec3(
		indices[faceId * 3],
		indices[(faceId * 3) + 1],
		indices[(faceId * 3) + 2]
	);
}

unsigned int Object3D::facesSize() {
	return indices.size() / 3;
}
