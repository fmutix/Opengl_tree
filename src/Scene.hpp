#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <glm/vec3.hpp>

#include "Shader.hpp"
#include "Camera.hpp"
#include "Phong.hpp"
#include "Object3D.hpp"

class Scene {
public:

    Scene(Camera& camera, Phong& phong,
    glm::vec3& lightColor, glm::vec3& objectColor);

    void uniform(Shader& shader);
    void uniformPhong(Shader& shader);

    void add(Object3D& obj);
    void display();

private:
    Camera& camera;
    Phong& phong;
    glm::vec3& lightColor;
    glm::vec3& objectColor;
    std::vector<Object3D> objects;

};

#endif // SCENE_HPP
