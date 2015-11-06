#include <glm/gtx/rotate_vector.hpp>

#include "Scene.hpp"

Scene::Scene() : phong_(Phong(0.1f, glm::vec3(1.0f, 2.0f, 3.0f), 2.0f)) {
		lightColor_ = glm::vec3(1.0f, 1.0f, 1.0f);
		objectColor_ = glm::vec3(1.0f, 0.2f, 0.2f);
	}

void Scene::uniform(Shader& shader) {
	uniformPhong(shader);
}

void Scene::uniformPhong(Shader& shader) {
	shader.setUniform("ambientIntensity", phong_.getAmbientIntensity());
	shader.setUniform("diffusePos", phong_.getDiffusePosition());
	shader.setUniform("specularIntensity", phong_.getSpecularIntensity());
	shader.setUniform("lightColor", lightColor_);
	shader.setUniform("objectColor", objectColor_);
}

void Scene::rotatePhong(float vx, float vy, glm::vec3 axis) {
	phong_.rotate(vx, vy, axis);
}

void Scene::add(Object3D& obj) {
	objects_.push_back(obj);
}

void Scene::display() {
	for (Object3D obj : objects_) {
		obj.display();
	}
}
