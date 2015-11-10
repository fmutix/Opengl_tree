#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/vec3.hpp>

#include "Object3D.hpp"

class Light : public Object3D {
public:
	Light();
	Light(
		std::string filename,
		glm::vec3 diffuse,
		float specular,
		glm::vec3 rayColor,
		glm::vec3 color
	);

	glm::vec3 getDiffusePosition();
	float getSpecularIntensity();
	glm::vec3 getRayColor();
	void rotate(float vx, float vy, glm::vec3 axis);

private:
	glm::vec3 diffusePosition_;
	float specularIntensity_;
	glm::vec3 rayColor_;
};

#endif // LIGHT_HPP
