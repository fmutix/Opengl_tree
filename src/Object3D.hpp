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
	Object3D(std::string filename);

	void init(std::string filename);

	GLuint getTexture();
	void setTexture(Texture& texture);

	void display();
	unsigned int facesSize();

private:
	GLuint vao;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint indexBuffer;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> indices;

	Texture texture;

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
