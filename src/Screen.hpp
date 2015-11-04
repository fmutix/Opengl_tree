#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "Shader.hpp"

class Screen {
public:
    Screen(float fovy, int width, int height, float zNear, float zFar);
    
    void reshape(int width, int height);
    void uniformProjectionMatrix(Shader& shader);
    
    int getWidth();
    int getHeight();
    
private:
    float fovy;
    int width;
    int height;
    float zNear;
    float zFar;
};

#endif // CAMERA_HPP
