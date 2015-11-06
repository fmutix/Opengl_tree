#include <glm/gtx/rotate_vector.hpp>

#include "Phong.hpp"

Phong::Phong(
	const float& ambient, const glm::vec3& diffuse, const float& specular) {

	ambientIntensity_ = ambient;
	diffusePosition_ = diffuse;
	specularIntensity_ = specular;
}

float Phong::getAmbientIntensity() {
	return ambientIntensity_;
}

glm::vec3 Phong::getDiffusePosition() {
	return diffusePosition_;
}

float Phong::getSpecularIntensity() {
	return specularIntensity_;
}

void Phong::rotate(float vx, float vy, glm::vec3 axis) {
	float angle = 4.0f * (fabs(vx) + fabs(vy));
	diffusePosition_ = glm::rotate(diffusePosition_, angle, axis);
}
