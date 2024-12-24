#pragma once
#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <GL/glew.h>

// 加载着色器程序
unsigned int loadShader(const char* vertexPath, const char* fragmentPath);

// 设置着色器中的矩阵（如投影、视图、模型矩阵等）
void setUniformMat4(unsigned int shaderProgram, const char* name, const float* value);

// 使用着色器
void useShader(unsigned int shaderProgram);

#endif // SHADER_UTILS_H
