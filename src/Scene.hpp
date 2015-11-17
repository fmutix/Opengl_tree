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
	void initApple();
	void initParticles();

	float randBounded(float min, float max);
	void uniformObjects(Shader& shader);
	void uniformLight(Shader& shader);

	void rotateLight(float angle, glm::vec3 axis);

	void add(Object3D& obj);
	void displayObjects(Shader& shader);
	void displayLight();

private:
	Light light_;
	float ambient_;
	glm::vec3 lightColor_;
	glm::vec3 objectColor_;
	Object3D appleMesh_;
	std::vector<Object3D> objects_;
	std::vector<Particle> particles_;
};

#endif // SCENE_HPP
