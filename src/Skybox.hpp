#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <vector>
#include <string>
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <glm/vec3.hpp>

#include "Shader.hpp"

class Skybox {
public:
	Skybox();
	Skybox(glm::vec3 position);
	void init(std::string filename);
	void initTexture();

	GLuint getTexture();

	float getScale();
	void setScale(float scale);

	void setUniform(Shader& shader);
	void display();
	unsigned int facesSize();

private:
	GLuint vao_;
	GLuint vertexBuffer_;
	GLuint indexBuffer_;
	GLuint texture_;

	int width_;
	int height_;
	unsigned char* image_;

	std::vector<glm::vec3> vertices_;
	std::vector<GLuint> indices_;

	glm::vec3 position_;
	float scale_;
	std::vector<std::string> cubeTextures_;

	void readMesh(std::string filename);
	void createObjectBuffers();
	glm::uvec3 getFace(unsigned int faceId);
};

#endif // SKYBOX_HPP
