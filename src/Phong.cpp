#include <glm/gtx/rotate_vector.hpp>

#include "Phong.hpp"

Phong::Phong(
    const float& ambient, const glm::vec3& diffuse, const float& specular) {
    
    ambientIntensity = ambient;
    diffusePosition = diffuse;
    specularIntensity = specular;
}

float Phong::getAmbientIntensity() {
    return ambientIntensity;
}

glm::vec3 Phong::getDiffusePosition() {
    return diffusePosition;
}

float Phong::getSpecularIntensity() {
    return specularIntensity;
}

void Phong::rotate(float vx, float vy, glm::vec3 axis) {
    float angle = 4.0f * (fabs(vx) + fabs(vy));
    diffusePosition = glm::rotate(diffusePosition, angle, axis);
}
