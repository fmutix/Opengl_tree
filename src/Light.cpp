#include <glm/gtx/rotate_vector.hpp>

#include "Light.hpp"

Light::Light(glm::vec3 diffuse, float specular, glm::vec3 color) {
	diffusePosition_ = diffuse;
	specularIntensity_ = specular;
	color_ = color;
}

glm::vec3 Light::getDiffusePosition() {
	return diffusePosition_;
}

float Light::getSpecularIntensity() {
	return specularIntensity_;
}

glm::vec3 Light::getColor() {
	return color_;
}

void Light::rotate(float vx, float vy, glm::vec3 axis) {
	float angle = 4.0f * (fabs(vx) + fabs(vy));
	diffusePosition_ = glm::rotate(diffusePosition_, angle, axis);
}
