#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <iostream>
#include <glm/vec3.hpp>

class Texture {
public:
	Texture();
	Texture(std::string name);

	GLuint getTexture();

private:
	GLuint texture_;
	int width_;
	int height_;
	unsigned char* image_;
};

#endif // TEXTURE_HPP
