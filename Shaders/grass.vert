#version 330 core
layout(location = 0) in vec3 aPos;       // 顶点位置
layout(location = 1) in vec2 aTexCoords; // 纹理坐标

out vec2 TexCoords; // 传递给片段着色器的纹理坐标

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// 控制地形高度的振幅和频率
uniform float amplitude; // 高度振幅
uniform float frequency; // 频率参数

// 平滑噪声：插值随机噪声
float smoothRandom(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // 四个角的随机值
    float a = fract(sin(dot(i + vec2(0.0, 0.0), vec2(12.9898, 78.233))) * 43758.5453);
    float b = fract(sin(dot(i + vec2(1.0, 0.0), vec2(12.9898, 78.233))) * 43758.5453);
    float c = fract(sin(dot(i + vec2(0.0, 1.0), vec2(12.9898, 78.233))) * 43758.5453);
    float d = fract(sin(dot(i + vec2(1.0, 1.0), vec2(12.9898, 78.233))) * 43758.5453);

    // 平滑插值
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

// 多层平滑噪声（fBM）
float fbm(vec2 st) {
    float total = 0.0;
    float persistence = 0.5; // 每层的振幅衰减因子
    float freq = frequency;
    float amp = 1.0;

    for (int i = 0; i < 4; ++i) { // 叠加 4 层平滑噪声
        total += smoothRandom(st * freq) * amp;
        freq *= 2.0;    // 频率倍增
        amp *= persistence; // 振幅衰减
    }
    return total;
}

void main() {
    // 计算分形噪声高度
    float height = amplitude * fbm(aPos.xz);

    // 添加一个轻微的扰动
    height += amplitude * 0.1 * smoothRandom(aPos.xz * 10.0);

    // 如果是底面或侧面，保持原始高度
    float newY = (aPos.y < 0.0) ? aPos.y : height;

    // 顶点的新位置
    vec3 newPos = vec3(aPos.x, newY, aPos.z);

    // 传递纹理坐标
    TexCoords = aTexCoords;

    // 应用 MVP 变换
    gl_Position = projection * view * model * vec4(newPos, 1.0);
}
