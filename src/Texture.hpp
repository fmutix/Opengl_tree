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
	GLuint texture;
	int width;
	int height;
	unsigned char* image;
};

#endif // TEXTURE_HPP
