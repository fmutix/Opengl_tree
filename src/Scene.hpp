#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <glm/vec3.hpp>

#include "Shader.hpp"
#include "Light.hpp"
#include "Object3D.hpp"

class Scene {
public:

	Scene();
	Scene(float ambient, Light light);

	void uniform(Shader& shader);
	void uniformLight(Shader& shader);
	
	void rotateLight(float vx, float vy, glm::vec3 axis);

	void add(Object3D& obj);
	void display(Shader& shader);

private:
	Light light_;
	float ambient_;
	glm::vec3 lightColor_;
	glm::vec3 objectColor_;
	std::vector<Object3D> objects_;
};

#endif // SCENE_HPP
