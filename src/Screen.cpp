#include <glm/gtc/matrix_transform.hpp>

#include "Screen.hpp"

Screen::Screen(float fovy, int width, int height, float zNear, float zFar) {
    this->fovy = fovy;
    this->width = width;
    this->height = height;
    this->zNear = zNear;
    this->zFar = zFar;
}

void Screen::reshape(int width, int height) {
    this->width = width;
    this->height = height;
    
    glViewport(0, 0, width, height);
}

void Screen::uniformProjectionMatrix(Shader& shader) {
    glm::mat4 projectionMatrix = glm::perspective(
        fovy, (float) width/height, zNear, zFar
    );
    shader.setUniform("projection", projectionMatrix);
}

int Screen::getWidth() { return width; }
int Screen::getHeight() { return height; }
