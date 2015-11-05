#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/vec3.hpp>

#include "Shader.hpp"

class Camera {
public:
    Camera(
        const glm::vec3& position, const glm::vec3& up,
        const glm::vec3& forward
    );

    glm::vec3 rotate(float vx, float vy);
    void rotate(float angle, glm::vec3 rotationAxis);
    void moveForwardBackward(float value);
    void moveLeftRight(float value);
    void reset();

    void uniformViewMatrix(Shader& shader);
    void uniformPosition(Shader& shader);

    glm::mat4 getViewMatrix();
    glm::vec3 getPosition();
    glm::vec3 getUp();
    glm::vec3 getForward();
    glm::vec3 getRight();

private:
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 forward;
    
    glm::vec3 initPosition;
    glm::vec3 initUp;
    glm::vec3 initForward;
    
    glm::vec3 right;
};

#endif // OBJECT3D_HPP
