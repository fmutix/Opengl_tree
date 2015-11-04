#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

Shader::Shader() {}

Shader::Shader(
    std::string vsFilename, std::string fragFilename, std::string gsFilename
) {
    init(vsFilename, fragFilename, gsFilename);
}

void Shader::init(
    std::string vsFilename, std::string fragFilename, std::string gsFilename
) {
    vertexShader = addShader(GL_VERTEX_SHADER, vsFilename);
    fragmentShader = addShader(GL_FRAGMENT_SHADER, fragFilename);
    
    if (!gsFilename.empty()) {
        geometryShader = addShader(GL_GEOMETRY_SHADER, gsFilename);
    }
    else {
        geometryShader = 0;
    }
    
    initProgram();
}

GLuint Shader::addShader(GLenum type, std::string filename) {
    std::string str = readShaderFile(filename);
    const GLchar* shaderCode = str.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        compilationErrors(shader);
        exit(EXIT_FAILURE);
    }
    
    return shader;
}


std::string Shader::readShaderFile (std::string filename) {
    std::ifstream ifs(filename, std::ifstream::in);
    
    if (!ifs) {
        std::cerr << "Unable to Open File " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();

    return ss.str();
}


void Shader::initProgram() {
    program = glCreateProgram();
    
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    if (geometryShader != 0) {
        glAttachShader(program, geometryShader);
    }
    
    glLinkProgram(program);
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        linkageErrors();
        exit(EXIT_FAILURE);
    }
}

void Shader::use() {
    glUseProgram(program);
}

GLuint Shader::uniformLocation(std::string uniformName) {
    return glGetUniformLocation(program, uniformName.c_str());
}

void Shader::setUniform(const std::string& name, GLuint value) {
    GLuint id = uniformLocation(name);
    glUniform1i(id, value);
}

void Shader::setUniform(const std::string& name, float value) {
    GLuint id = uniformLocation(name);
    glUniform1f(id, value);
}

void Shader::setUniform(const std::string& name, glm::vec3 value) {
    GLuint id = uniformLocation(name);
    glUniform3fv(id, 1, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, glm::mat3 value) {
    GLuint id = uniformLocation(name);
    glUniformMatrix3fv(id, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, glm::mat4 value) {
    GLuint id = uniformLocation(name);
    glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::compilationErrors(GLint shader) {
    GLint length;
    
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    GLchar* log = new GLchar[length + 1];
    
    glGetShaderInfoLog(shader, length, &length, log);
    std::cout << "Compile Error, Log Below" << std::endl
         << log << std::endl;
    
    delete[] log;
}

void Shader::linkageErrors() {
    GLint length;
    
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    GLchar* log = new GLchar[length + 1];
    
    glGetProgramInfoLog(program, length, &length, log);
    std::cout << "Compile Error, Log Below" << std::endl
         << log << std::endl;
    
    delete[] log;
}
