#ifndef GRASS_H
#define GRASS_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Grass {
public:
    Grass(const std::string& texturePath);
    ~Grass();

    void setAmplitude(float amp);    // 设置地形高度振幅
    void setFrequency(float freq);   // 设置地形起伏频率
    void setThickness(float thick);  // 设置地形的厚度

    void render(const glm::mat4& view, const glm::mat4& projection);

private:
    unsigned int VAO, VBO, EBO;
    unsigned int textureID;
    unsigned int shaderProgram;

    float amplitude;  // 地形高度振幅
    float frequency;  // 地形起伏频率
    float thickness;  // 地形厚度

    void setupPlane();
    void loadTexture(const std::string& texturePath);
};

#endif // GRASS_H
