#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <glm/vec3.hpp>

#include "Shader.hpp"
#include "Phong.hpp"
#include "Object3D.hpp"

class Scene {
public:

	Scene();

	void uniform(Shader& shader);
	void uniformPhong(Shader& shader);
	
	void rotatePhong(float vx, float vy, glm::vec3 axis);

	void add(Object3D& obj);
	void display();

private:
	Phong phong_;
	glm::vec3 lightColor_;
	glm::vec3 objectColor_;
	std::vector<Object3D> objects_;
};

#endif // SCENE_HPP
