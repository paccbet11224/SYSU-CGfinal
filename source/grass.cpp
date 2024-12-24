#include "grass.h"
#include "shader_utils.h"
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Grass::Grass(const std::string& texturePath)
    : amplitude(2.0f), frequency(0.1f), thickness(3.0f) {
    setupPlane();
    loadTexture(texturePath);
    shaderProgram = loadShader("Shaders/grass.vert", "Shaders/grass.frag");
}

Grass::~Grass() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &textureID);
}

void Grass::setAmplitude(float amp) { amplitude = amp; }
void Grass::setFrequency(float freq) { frequency = freq; }
void Grass::setThickness(float thick) { thickness = thick; setupPlane(); }

void Grass::setupPlane() {
    const int gridSize = 100; // 网格大小
    const float gridSpacing = 1.0f;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // 生成顶面和底面顶点
    for (int z = 0; z <= gridSize; ++z) {
        for (int x = 0; x <= gridSize; ++x) {
            // 顶面
            vertices.push_back(x * gridSpacing - gridSize / 2.0f);
            vertices.push_back(0.0f);
            vertices.push_back(z * gridSpacing - gridSize / 2.0f);
            vertices.push_back((float)x / gridSize);
            vertices.push_back((float)z / gridSize);

            // 底面
            vertices.push_back(x * gridSpacing - gridSize / 2.0f);
            vertices.push_back(-thickness);
            vertices.push_back(z * gridSpacing - gridSize / 2.0f);
            vertices.push_back((float)x / gridSize);
            vertices.push_back((float)z / gridSize);
        }
    }

    // 生成顶面、底面和侧面索引
    for (int z = 0; z < gridSize; ++z) {
        for (int x = 0; x < gridSize; ++x) {
            int topLeft = z * (gridSize + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * (gridSize + 1) + x;
            int bottomRight = bottomLeft + 1;

            // 顶面
            indices.push_back(topLeft * 2);
            indices.push_back(bottomLeft * 2);
            indices.push_back(topRight * 2);

            indices.push_back(topRight * 2);
            indices.push_back(bottomLeft * 2);
            indices.push_back(bottomRight * 2);

            // 底面（反向）
            indices.push_back(topLeft * 2 + 1);
            indices.push_back(topRight * 2 + 1);
            indices.push_back(bottomLeft * 2 + 1);

            indices.push_back(topRight * 2 + 1);
            indices.push_back(bottomRight * 2 + 1);
            indices.push_back(bottomLeft * 2 + 1);

            // 侧面
            indices.push_back(topLeft * 2);
            indices.push_back(topLeft * 2 + 1);
            indices.push_back(topRight * 2);

            indices.push_back(topRight * 2);
            indices.push_back(topLeft * 2 + 1);
            indices.push_back(topRight * 2 + 1);
        }
    }

    // OpenGL 设置
    unsigned int VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Grass::loadTexture(const std::string& texturePath) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }
}

void Grass::render(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(shaderProgram);

    // 传递 MVP 矩阵和地形参数
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    glUniform1f(glGetUniformLocation(shaderProgram, "amplitude"), amplitude);
    glUniform1f(glGetUniformLocation(shaderProgram, "frequency"), frequency);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shaderProgram, "grassTexture"), 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6 * 100 * 100 * 2, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
