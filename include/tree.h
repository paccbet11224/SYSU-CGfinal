#ifndef TREE_H
#define TREE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include "model.h"

class Tree {
public:
    Tree(const std::string& objPath, const std::string& mtlPath);
    void render(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& modelMatrix);

private:
    Model* model;
    GLuint shaderProgram;

    void loadShaders();
};

#endif
