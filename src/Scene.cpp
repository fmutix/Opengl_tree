#include <glm/gtx/rotate_vector.hpp>

#include "Scene.hpp"

Scene::Scene() : phong_(Phong(glm::vec3(1.0f, 1.0f, 0.5f), 2.0f)) {
	ambient_ = 0.1;
	lightColor_ = glm::vec3(1.0f, 1.0f, 1.0f);
}

Scene::Scene(float ambient) : phong_(Phong(glm::vec3(1.0f, 0.0f, 0.0f), 2.0f)) {
	ambient_ = ambient;
	lightColor_ = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Scene::uniform(Shader& shader) {
	shader.setUniform("ambient", ambient_);
	uniformPhong(shader);
}

void Scene::uniformPhong(Shader& shader) {
	
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
