#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <string>
#include <GL/glew.h>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

class Shader {
public:
    Shader();
    Shader(
        std::string vsFilename, std::string fragFilename,
        std::string gsFilename = ""
    );
    
    void init(
        std::string vsFilename, std::string fragFilename,
        std::string gsFilename = ""
    );
    void use();
    
    GLuint uniformLocation(std::string uniformName);
    void setUniform(const std::string& name, GLuint value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, glm::vec3 value);
    void setUniform(const std::string& name, glm::mat3 value);
    void setUniform(const std::string& name, glm::mat4 value);
    
private:
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint geometryShader;
    GLuint program;
    
    GLuint addShader(GLenum type, std::string filename);
    std::string readShaderFile(std::string filename);
    void initProgram();
    void compilationErrors(GLint shader);
    void linkageErrors();
};

#endif // SHADERS_HPP
