#ifndef SHADER_H
#define SHADER_H
#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//using namespace std;

#define INVALID_UNIFORM_LOCATION -1

class Shader {
public:
    // 着色器程序 ID
    GLuint ID;

    // 构造器，加载着色器
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath);
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath,
        const GLchar* varyings[], int count);
    // 使用着色器
    void use();
    // 设置统一变量（Uniforms）
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    // 加载并编译着色器
    std::string loadShaderSource(const std::string& path);
    void compileShader(const char* source, GLuint shaderID);
};

#endif
#pragma once
