#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <cstdlib>
#include <glm/vec3.hpp>

#include "Shader.hpp"
#include "Light.hpp"
#include "Object3D.hpp"
#include "Particle.hpp"

class Scene {
public:

	Scene();
	Scene(Light light);
	Scene(float ambient, Light light);
	void initMeshes();
	void initParticles();

	void uniformObjects(Shader& shader);
	void uniformLight(Shader& shader);

	void applesReady();
	void leavesReady();
	void rotateLight(float angle, glm::vec3 axis);

	void add(Object3D& obj);
	void displayObjects(Shader& shader, int season);
	void displayLight();

	Light getLight();

private:
	Light light_;
	float ambient_;
	glm::vec3 lightColor_;
	glm::vec3 objectColor_;
	Object3D appleMesh_;
	Object3D leafMesh_;
	Object3D treeMesh_;
	std::vector<Particle> apples_;
	std::vector<Particle> leaves_;
};

#endif // SCENE_HPP
