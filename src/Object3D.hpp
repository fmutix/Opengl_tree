#ifndef OBJECT3D_HPP
#define OBJECT3D_HPP

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/vec3.hpp>

#include "Texture.hpp"

class Object3D {
public:
	Object3D();
	Object3D(std::string filename, glm::vec3 position, std::string tex);
	Object3D(std::string filename, glm::vec3 position, glm::vec3 color);

	void init(std::string filename);

	GLuint getTexture();
	void setTexture(Texture& texture);
	glm::vec3 getColor();
	glm::vec3 getPosition();

	bool hasTexture();
	void display();
	unsigned int facesSize();

private:
	GLuint vao_;
	GLuint vertexBuffer_;
	GLuint normalBuffer_;
	GLuint indexBuffer_;

	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::vector<GLuint> indices_;

	glm::vec3 position_;
	glm::vec3 color_;
	Texture texture_;
	bool textured_;

	void readMesh(std::string filename);
	glm::vec3 computeTriangleNormal(unsigned int faceId);
	glm::vec3 computeTriangleNormal(
		const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3
	);
	void normalizeNormals();
	void computeNormals();
	void createObjectBuffers();
	glm::uvec3 getFace(unsigned int faceId);
};


#endif // OBJECT3D_HPP
