#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    ~Skybox();
    void render(const glm::mat4& view, const glm::mat4& projection);

private:
    unsigned int loadCubemap(const std::vector<std::string>& faces);
    void setupSkybox();

    unsigned int VAO, VBO;
    unsigned int cubemapTexture;
    unsigned int shaderProgram;
};

#endif
