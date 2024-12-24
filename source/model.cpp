#include "model.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stb_image.h>

GLuint Model::loadTexture(const std::string& texturePath) {
    std::cout << "Loading texture: " << texturePath << std::endl; // 打印纹理路径
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        std::cout << "Texture loaded successfully: " << texturePath << std::endl; // 确认纹理加载成功
        GLenum format = (nrComponents == 4) ? GL_RGBA : GL_RGB;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}

Model::Model(const std::string& objPath, const std::string& mtlPath) {
    loadModeltwo(objPath, mtlPath);
    setupMesh();
}

void Model::loadModelone(const std::string& objPath, const std::string& mtlPath) {
    std::ifstream mtlFile(mtlPath);
    // 加载 MTL 文件，设置材质
    std::string line, currentMaterial;
    while (std::getline(mtlFile, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "newmtl") {
            ss >> currentMaterial;
            std::cout << "Current Material: " << currentMaterial << std::endl;  // 输出当前材质名称
        }
        else if (prefix == "map_Kd") {
            std::string texturePath;
            ss >> texturePath;
            if (currentMaterial == "_Vegetation_Bark_Maple_1") {
                barkMaterial.textureID = loadTexture("Assets/Tree/" + texturePath);
            }
            else if (currentMaterial == "_40") {
                leafMaterial.textureID = loadTexture("Assets/Tree/" + texturePath);
            }
            else if (currentMaterial == "stone") {
                stoneMaterial.textureID = loadTexture("Assets/Rock/" + texturePath);
            }
        }
    }

    // 加载 OBJ 文件
    std::ifstream objFile(objPath);
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texcoords;

    while (std::getline(objFile, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            glm::vec3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);
        }
        else if (prefix == "vt") {
            glm::vec2 tex;
            ss >> tex.x >> tex.y;
            temp_texcoords.push_back(tex);
        }
        else if (prefix == "usemtl") {
            ss >> currentMaterial;
        }
        else if (prefix == "f") {
            std::string vertexStr[3];
            ss >> vertexStr[0] >> vertexStr[1] >> vertexStr[2];
            for (int i = 0; i < 3; ++i) {
                unsigned int posIdx, texIdx;
                sscanf_s(vertexStr[i].c_str(), "%d/%d", &posIdx, &texIdx);

                Vertex vert;
                vert.Position = temp_positions[posIdx - 1];
                vert.TexCoords = temp_texcoords[texIdx - 1];

                if (currentMaterial == "_Vegetation_Bark_Maple_1") {
                    vertices_bark.push_back(vert);
                }
                else if (currentMaterial == "_40") {
                    vertices_leaves.push_back(vert);
                }
                else if (currentMaterial == "stone") {
                    vertices_stone.push_back(vert);  // 新的顶点数据
                }
            }
        }
    }
}

void Model::loadModeltwo(const std::string& objPath, const std::string& mtlPath) {
    std::ifstream mtlFile(mtlPath);
    // 加载 MTL 文件，设置材质
    std::string line, currentMaterial;
    while (std::getline(mtlFile, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "newmtl") {
            ss >> currentMaterial;
            std::cout << "Current Material: " << currentMaterial << std::endl;  // 输出当前材质名称
        }
        else if (prefix == "map_Kd") {
            std::string texturePath;
            ss >> texturePath;
            if (currentMaterial == "_Vegetation_Bark_Maple_1") {
                barkMaterial.textureID = loadTexture("Assets/Tree/" + texturePath);
            }
            else if (currentMaterial == "_40") {
                leafMaterial.textureID = loadTexture("Assets/Tree/" + texturePath);
            }
            else if (currentMaterial == "stone") {
                stoneMaterial.textureID = loadTexture("Assets/Rock/" + texturePath);
            }
        }
    }

    std::ifstream file(objPath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open OBJ file!" << std::endl;
        return;
    }

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texcoords;
    //std::string currentMaterial;
    //std::string line;

    // 第一遍读取所有顶点和纹理坐标
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            glm::vec3 position;
            ss >> position.x >> position.y >> position.z;
            temp_positions.push_back(position);
        }
        else if (prefix == "vt") {
            glm::vec2 texcoord;
            ss >> texcoord.x >> texcoord.y;
            temp_texcoords.push_back(texcoord);
        }
    }

    // 第二遍读取所有的面数据
    file.clear();
    file.seekg(0, std::ios::beg);  // 文件指针重新回到开头

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "f") {
            std::string vertexStr[3];
            ss >> vertexStr[0] >> vertexStr[1] >> vertexStr[2];
            for (int i = 0; i < 3; ++i) {
                unsigned int posIdx, texIdx;
                sscanf_s(vertexStr[i].c_str(), "%d/%d", &posIdx, &texIdx);

                Vertex vert;
                vert.Position = temp_positions[posIdx - 1];  // 索引从 1 开始，数组从 0 开始
                vert.TexCoords = temp_texcoords[texIdx - 1];

                // 根据材料区分顶点数据存放
                if (currentMaterial == "_Vegetation_Bark_Maple_1") {
                    vertices_bark.push_back(vert);
                }
                else if (currentMaterial == "_40") {
                    vertices_leaves.push_back(vert);
                }
                else if (currentMaterial == "stone") {
                    vertices_stone.push_back(vert);
                }
            }
        }
    }
    file.close();
}

void Model::setupMesh() {
    // 设置树干 VAO/VBO
    //glGenVertexArrays(1, &VAO_bark);
    //glGenBuffers(1, &VBO_bark);
    //glBindVertexArray(VAO_bark);
    /*glBindBuffer(GL_ARRAY_BUFFER, VBO_bark);
    glBufferData(GL_ARRAY_BUFFER, vertices_bark.size() * sizeof(Vertex), &vertices_bark[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(1);*/

    // 设置树叶 VAO/VBO
    /*glGenVertexArrays(1, &VAO_leaves);
    glGenBuffers(1, &VBO_leaves);
    glBindVertexArray(VAO_leaves);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_leaves);
    glBufferData(GL_ARRAY_BUFFER, vertices_leaves.size() * sizeof(Vertex), &vertices_leaves[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(1);*/

    // 设置石头 VAO/VBO
    glGenVertexArrays(1, &VAO_stone);  // 创建石头的 VAO
    glGenBuffers(1, &VBO_stone);      // 创建石头的 VBO
    glBindVertexArray(VAO_stone);     // 绑定石头的 VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_stone); // 绑定石头的 VBO
    glBufferData(GL_ARRAY_BUFFER, vertices_stone.size() * sizeof(Vertex), &vertices_stone[0], GL_STATIC_DRAW); // 上传顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position)); // 设置位置属性
    glEnableVertexAttribArray(0); // 启用位置属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)); // 设置纹理坐标属性
    glEnableVertexAttribArray(1); // 启用纹理坐标属性
}

void Model::Render() {
    /*glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, barkMaterial.textureID);
    glBindVertexArray(VAO_bark);
    glDrawArrays(GL_TRIANGLES, 0, vertices_bark.size());

    glBindTexture(GL_TEXTURE_2D, leafMaterial.textureID);
    glBindVertexArray(VAO_leaves);
    glDrawArrays(GL_TRIANGLES, 0, vertices_leaves.size());*/

    glBindTexture(GL_TEXTURE_2D, stoneMaterial.textureID); // 石头的纹理
    glBindVertexArray(VAO_stone);
    glDrawArrays(GL_TRIANGLES, 0, vertices_stone.size());
}
