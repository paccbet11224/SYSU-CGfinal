#include "rock.h"
#include <glm/gtc/type_ptr.hpp>
#include "shader_utils.h"
#include <iostream>

Rock::Rock(const std::string& objPath, const std::string& mtlPath) {
    model = new Model(objPath, mtlPath);
    loadShaders();
}

void Rock::loadShaders() {
    shaderProgram = loadShader("Shaders/rock.vert", "Shaders/rock.frag");
}

void Rock::render(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& modelMatrix) {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    model->Render();
}
