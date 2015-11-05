#include <glm/gtx/rotate_vector.hpp>

#include "Scene.hpp"

Scene::Scene() : phong(Phong(0.1f, glm::vec3(1.0f, 2.0f, 3.0f), 2.0f)) {
		lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		objectColor = glm::vec3(1.0f, 0.2f, 0.2f);
    }

void Scene::uniform(Shader& shader) {
    uniformPhong(shader);
}

void Scene::uniformPhong(Shader& shader) {
    shader.setUniform("ambientIntensity", phong.getAmbientIntensity());
    shader.setUniform("diffusePos", phong.getDiffusePosition());
    shader.setUniform("specularIntensity", phong.getSpecularIntensity());
    shader.setUniform("lightColor", lightColor);
    shader.setUniform("objectColor", objectColor);
}

void Scene::rotatePhong(float vx, float vy, glm::vec3 axis) {
    phong.rotate(vx, vy, axis);
}

void Scene::add(Object3D& obj) { objects.push_back(obj); }

void Scene::display() {
    for (Object3D obj : objects) {
        obj.display();
    }
}
