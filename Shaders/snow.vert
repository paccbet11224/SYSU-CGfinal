#version 330 core

layout (location = 0) in float Type0[];
layout (location = 1) in vec3 Position0[];
layout (location = 2) in vec3 Velocity0[];
layout (location = 3) in float Age0[];
layout (location = 4) in float Size0[];

out float Type1;
out vec3 Position1;
out vec3 Velocity1;
out float Age1;
out float Size1;

uniform float gDeltaTimeMillis;
uniform float gTime;
uniform sampler1D gRandomTexture;
uniform float MAX_SIZE;
uniform float MIN_SIZE;
uniform vec3 MAX_VELOC;
uniform vec3 MIN_VELOC;
uniform float MAX_LAUNCH;
uniform float MIN_LAUNCH;
uniform vec3 light_position;
uniform vec3 camera_position;
uniform mat4 view;
uniform mat4 projection;

#define PARTICLE_TYPE_LAUNCHER 0.0f // 发射器
#define PARTICLE_TYPE_SHELL 1.0f    // 运动粒子

vec3 GetRandomDir(float TexCoord);
vec3 Rand(float TexCoord);

void main()
{
    float Age = Age0[0] - gDeltaTimeMillis;
    float speedRate = 0.1f;
    if(Type0[0] == PARTICLE_TYPE_LAUNCHER) {
        if(Age <= 0.0f) {
            // 发射第二级粒子
            Type1 = PARTICLE_TYPE_SHELL;
            Position1 = Position0[0];
            vec3 Dir = GetRandomDir(Age0[0] + gTime);
            // 保证方向向下
            Dir.y = min(Dir.y, -0.5f);
            Velocity1 = normalize(Dir) / speedRate;
            Age1 = Age0[0];
            Size1 = (MAX_SIZE - MIN_SIZE) * Rand(Age0[0] + gTime).x + MIN_SIZE;
            // EmitVertex(); 这里将在几何着色器中处理
            // EndPrimitive();
            // 重置发射器的年龄
            Age = (MAX_LAUNCH - MIN_LAUNCH) * Rand(Age0[0]).y + MIN_LAUNCH;
        }
        Type1 = PARTICLE_TYPE_LAUNCHER;
        Position1 = Position0[0];
        Velocity1 = Velocity0[0];
        Age1 = Age;
        Size1 = 0.0f;
        // EmitVertex(); 这里将在几何着色器中处理
        // EndPrimitive();
    } else {
        if(Position0[0].y > 0.0f) {
            // 单位转秒
            float DeltaTimeSecs = gDeltaTimeMillis / 1000.0f;
            // 属性变化量
            vec3 DeltaP = Velocity0[0] * DeltaTimeSecs;
            vec3 DeltaV = DeltaTimeSecs * vec3(10.0, -9.81, 0.0);
            Type1 = PARTICLE_TYPE_SHELL;
            Position1 = Position0[0] + DeltaP;
            Velocity1 = Velocity0[0] + DeltaV;
            Age1 = Age;
            Size1 = Size0[0];
            // EmitVertex(); 这里将在几何着色器中处理
            // EndPrimitive();
        }
    }
}

vec3 GetRandomDir(float TexCoord)
{
    vec3 Dir = texture(gRandomTexture, TexCoord).xyz;
    Dir -= vec3(0.5, 0.5, 0.5);
    return Dir;
}

vec3 Rand(float TexCoord)
{
    vec3 ret = texture(gRandomTexture, TexCoord).xyz;
    return ret; 
}
