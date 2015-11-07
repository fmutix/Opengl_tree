#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/vec3.hpp>

class Light {
public:
	Light(
		glm::vec3 diffuse,
		float specular,
		glm::vec3 color
	);

	glm::vec3 getDiffusePosition();
	float getSpecularIntensity();
	glm::vec3 getColor();
	void rotate(float vx, float vy, glm::vec3 axis);

private:
	glm::vec3 diffusePosition_;
	float specularIntensity_;
	glm::vec3 color_;
};

#endif // LIGHT_HPP
