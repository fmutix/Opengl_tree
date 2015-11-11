#include <glm/gtx/rotate_vector.hpp>

#include "Light.hpp"

Light::Light() {}

Light::Light(
	std::string filename, glm::vec3 diffuse, float specular,
	glm::vec3 rayColor, glm::vec3 color
) : Object3D(filename, diffuse, color) {
	diffusePosition_ = diffuse;
	specularIntensity_ = specular;
	rayColor_ = rayColor;
}

glm::vec3 Light::getDiffusePosition() {
	return diffusePosition_;
}

float Light::getSpecularIntensity() {
	return specularIntensity_;
}

glm::vec3 Light::getRayColor() {
	return rayColor_;
}

void Light::rotate(float angle, glm::vec3 axis) {
//	float angle = 4.0f * (fabs(vx) + fabs(vy));
//	diffusePosition_ = glm::rotate(diffusePosition_, angle, axis);
	diffusePosition_ = glm::rotate(diffusePosition_, angle, axis);
}
