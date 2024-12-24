#version 330 core
out vec4 FragColor;

in vec2 TexCoords; // 从顶点着色器传来的纹理坐标

uniform sampler2D grassTexture; // 草地纹理

void main() {
    // 从纹理中采样颜色
    FragColor = texture(grassTexture, TexCoords);
}
