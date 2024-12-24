#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = aPos; // 将立方体的顶点位置传递给片段着色器
    gl_Position = projection * view * vec4(aPos, 1.0); // 将顶点转换到裁剪空间
}
