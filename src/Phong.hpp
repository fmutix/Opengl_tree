#ifndef PHONG_HPP
#define PHONG_HPP

#include <glm/vec3.hpp>

class Phong {
public:
	Phong(
		const float& ambient,
		const glm::vec3& diffuse,
		const float& specular
	);

	float getAmbientIntensity();
	glm::vec3 getDiffusePosition();
	float getSpecularIntensity();
	void rotate(float vx, float vy, glm::vec3 axis);

private:
	float ambientIntensity_;
	glm::vec3 diffusePosition_;
	float specularIntensity_;
};

#endif // PHONG_HPP
