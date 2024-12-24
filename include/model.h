#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 Position;  // 顶点位置
    glm::vec2 TexCoords; // 纹理坐标
};

struct Material {
    GLuint textureID;    // 纹理ID
};

class Model {
public:
    Model(const std::string& objPath, const std::string& mtlPath);
    void Render();

private:
    std::vector<Vertex> vertices_bark;   // 树干顶点数据
    std::vector<Vertex> vertices_leaves; // 树叶顶点数据
    std::vector<Vertex> vertices_stone;

    GLuint VAO_bark, VBO_bark;           // 树干 VAO 和 VBO
    GLuint VAO_leaves, VBO_leaves;       // 树叶 VAO 和 VBO
    GLuint VAO_stone, VBO_stone;

    Material barkMaterial;
    Material leafMaterial;
    Material stoneMaterial;

    void loadModelone(const std::string& objPath, const std::string& mtlPath);
    void loadModeltwo(const std::string& objPath, const std::string& mtlPath);
    GLuint loadTexture(const std::string& texturePath);
    void setupMesh();
};

#endif
