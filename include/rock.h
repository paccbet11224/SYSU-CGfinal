#ifndef ROCK_H
#define ROCK_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include "model.h"  // 假设 Model 是一个封装了模型加载和渲染的类

class Rock {
public:
    // 构造函数，接受 OBJ 路径和材质路径
    Rock(const std::string& objPath, const std::string& mtlPath);

    // 渲染函数，接受视图矩阵、投影矩阵和模型矩阵
    void render(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& modelMatrix);

private:
    // 用于加载和渲染模型的 Model 类实例
    Model* model;

    // 着色器程序的 ID
    GLuint shaderProgram;

    // 用于存储光照信息、颜色等着色器变量的 uniform 位置
    GLuint lightColorLocation;  // 光源颜色的位置
    GLuint objectColorLocation; // 物体颜色的位置

    // 加载着色器的方法
    void loadShaders();
};

#endif // ROCK_H
