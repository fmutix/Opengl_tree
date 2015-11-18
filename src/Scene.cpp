#include <glm/gtx/rotate_vector.hpp>

#include "Scene.hpp"
#include "Utils.hpp"

const int NB_PARTICLE = 20;

Scene::Scene() {}

Scene::Scene(Light light) : light_(light) {
	ambient_ = 0.1;
	initApple();
	initParticles();
}

Scene::Scene(float ambient, Light light) : light_(light) {
	ambient_ = ambient;
	initApple();
	initParticles();
}

void Scene::initApple() {
	appleMesh_ = Object3D(
		"res/obj/apple.ctm",
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);
}

void Scene::initParticles() {
	for (int i = 0; i < NB_PARTICLE; i++) {
		glm::vec3 position(
			Utils::randBounded(-2, 2),
			Utils::randBounded(0.5, 1.0),
			Utils::randBounded(-2, 2)
		);
		float fade = Utils::randBounded(0.0001f, 0.001f);
		particles_.push_back(Particle(0.2f, 1.0f, fade, position));
	}
}

void Scene::uniformObjects(Shader& shader) {
	shader.setUniform("ambient", ambient_);
	shader.setUniform("diffusePos", light_.getDiffusePosition());
	shader.setUniform("specularIntensity", light_.getSpecularIntensity());
	shader.setUniform("lightColor", light_.getRayColor());
}

void Scene::uniformLight(Shader& shader) {
	shader.setUniform("objectColor", light_.getColor());
	shader.setUniform("objectPos", light_.getDiffusePosition());
	shader.setUniform("objectScale", light_.getScale());
}

void Scene::rotateLight(float angle, glm::vec3 axis) {
	light_.rotate(angle, axis);
}

void Scene::add(Object3D& obj) {
	objects_.push_back(obj);
}

void Scene::displayObjects(Shader& shader) {
	for (Particle& p : particles_) {
		p.process();
		if (p.getAlive()) {
			shader.setUniform("objectColor", appleMesh_.getColor());
			shader.setUniform("objectPos", p.getPosition());
			shader.setUniform("objectScale", p.getAgeMax() - p.getAge());
			shader.setUniform("objectHasTex", (GLuint)appleMesh_.hasTexture());
			appleMesh_.display();
		}
		else {
			p.live();
		}
	}
}

void Scene::displayLight() {
	light_.display();
}
