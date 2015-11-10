#include <glm/gtx/rotate_vector.hpp>

#include "Scene.hpp"

Scene::Scene() {}

Scene::Scene(Light light) : light_(light) {
	ambient_ = 0.1;
}

Scene::Scene(float ambient, Light light) : light_(light) {
	ambient_ = ambient;
}

void Scene::uniform(Shader& shader) {
	shader.setUniform("ambient", ambient_);
	uniformLight(shader);
}

void Scene::uniformLight(Shader& shader) {
	
	shader.setUniform("diffusePos", light_.getDiffusePosition());
	shader.setUniform("specularIntensity", light_.getSpecularIntensity());
	shader.setUniform("lightColor", light_.getRayColor());
}

void Scene::rotateLight(float vx, float vy, glm::vec3 axis) {
	light_.rotate(vx, vy, axis);
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
	shader.setUniform("objectColor", light_.getColor());
	shader.setUniform("objectModel", light_.getPosition());
	light_.display();
}
