#include <glm/gtx/rotate_vector.hpp>

#include "Scene.hpp"
#include "Utils.hpp"

const int NB_APPLE = 20;
const int NB_LEAF = 50;

Scene::Scene() {}

Scene::Scene(Light light) : light_(light) {
	ambient_ = 0.1;
	initMeshes();
	initParticles();
}

Scene::Scene(float ambient, Light light) : light_(light) {
	ambient_ = ambient;
	initMeshes();
	initParticles();
}

void Scene::initMeshes() {
	appleMesh_ = Object3D(
		"res/obj/apple.ctm",
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);
	leafMesh_ = Object3D(
		"res/obj/apple.ctm",
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	treeMesh_ = Object3D(
		"res/obj/tree.ctm",
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.44f, 0.27f, 0.14f)
	);
	treeMesh_.setScale(0.2f);
}

void Scene::initParticles() {
	for (int i = 0; i < NB_APPLE; i++) {
		glm::vec3 position(
			Utils::randBounded(-1, 1),
			Utils::randBounded(0.5, 1.0),
			Utils::randBounded(-1, 1)
		);
		float fade = Utils::randBounded(0.001f, 0.01f);
		apples_.push_back(Particle(0.05f, 1.0f, fade, position));
	}

	for (int i = 0; i < NB_LEAF; i++) {
		glm::vec3 position(
			Utils::randBounded(-1, 1),
			Utils::randBounded(0.5, 1.0),
			Utils::randBounded(-1, 1)
		);
		float fade = Utils::randBounded(0.001f, 0.01f);
		leaves_.push_back(Particle(0.05f, 1.0f, fade, position));
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

void Scene::displayObjects(Shader& shader, int season) {
	shader.setUniform("objectColor", treeMesh_.getColor());
	shader.setUniform("objectPos", treeMesh_.getPosition());
	shader.setUniform("objectScale", treeMesh_.getScale());
	shader.setUniform("objectHasTex", (GLuint)treeMesh_.hasTexture());
	treeMesh_.display();
	for (Particle& p : apples_) {
		p.process();
		if (p.getAlive()) {
			shader.setUniform("objectColor", appleMesh_.getColor());
			shader.setUniform("objectPos", p.getPosition());
			shader.setUniform("objectScale", p.getAgeMax() - p.getAge());
			shader.setUniform("objectHasTex", (GLuint)appleMesh_.hasTexture());
			appleMesh_.display();
		}
		else if (season == 3) {
			p.live();
		}
	}
	for (Particle& p : leaves_) {
		p.process();
		if (p.getAlive()) {
			shader.setUniform("objectColor", leafMesh_.getColor());
			shader.setUniform("objectPos", p.getPosition());
			shader.setUniform("objectScale", p.getAgeMax() - p.getAge());
			shader.setUniform("objectHasTex", (GLuint)leafMesh_.hasTexture());
			appleMesh_.display();
		}
		else if (season	== 1){
			p.live();
		}
	}
}

void Scene::displayLight() {
	light_.display();
}

Light Scene::getLight() {
	return light_;
}
