#include <glm/gtx/rotate_vector.hpp>

#include "Scene.hpp"

Scene::Scene(Phong& phong,
    glm::vec3& lightColor, glm::vec3& objectColor) : 
    phong(phong), lightColor(lightColor),
    objectColor(objectColor) {}

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

void Scene::add(Object3D& obj) { objects.push_back(obj); }

void Scene::display() {
    for (Object3D obj : objects) {
        obj.display();
    }
}
