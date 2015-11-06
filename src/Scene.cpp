#include <glm/gtx/rotate_vector.hpp>

#include "Scene.hpp"

Scene::Scene() : phong_(Phong(0.1f, glm::vec3(1.0f, 2.0f, 3.0f), 2.0f)) {
		lightColor_ = glm::vec3(1.0f, 1.0f, 1.0f);
	}

void Scene::uniform(Shader& shader) {
	uniformPhong(shader);
}

void Scene::uniformPhong(Shader& shader) {
	shader.setUniform("ambientIntensity", phong_.getAmbientIntensity());
	shader.setUniform("diffusePos", phong_.getDiffusePosition());
	shader.setUniform("specularIntensity", phong_.getSpecularIntensity());
	shader.setUniform("lightColor", lightColor_);
}

void Scene::rotatePhong(float vx, float vy, glm::vec3 axis) {
	phong_.rotate(vx, vy, axis);
}

void Scene::add(Object3D& obj) {
	objects_.push_back(obj);
}

void Scene::display(Shader& shader) {
	for (Object3D obj : objects_) {
		shader.setUniform("objectColor", obj.getColor());
		shader.setUniform("objectModel", obj.getPosition());
		obj.display();
	}
}
