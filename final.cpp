#include "skybox.h" // 天空盒
#include "grass.h"  // 草地
#include "camera.h" // 摄像机
#include "tree.h"   // 树木
#include "rock.h"
#include "Snow.h"
#include "shader.h"
#include "word.h"
#include "stb_image.h"
#include "drawable.h"
#include "shader_helper.h"
#include "particle.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include "shader_utils.h"
#include <GLAux.h>
#include <fstream>   // 用于 ifstream 和文件操作
#include <sstream>   // 用于 stringstream
#include <string>    // 用于 string 类
#include <vector>    // 用于 vector 容器

GLuint texture[2];//用于装纹理名称的数组
float angle = 0;
// 屏幕宽高
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
// 全局变量
glm::mat4 projection;
glm::mat4 view;
//Skybox skybox;  // 假设你已经定义了一个 Skybox 类
//Grass grass;    // 假设你已经定义了一个 Grass 类
//Rock rock;      // 假设你已经定义了一个 Rock 类
// 按键状态
bool keys[256]; // 用于存储普通键的状态
bool specialKeys[256]; // 用于存储特殊键（如方向键）的状态


void framebuffer_size_callback(int width, int height) {
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
}

// 摄像机实例
Camera camera(glm::vec3(-10.0f, 20.0f, 20.0f)); // 摄像机初始位置
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

std::vector<Drawable*> lst_drawable;

// 鼠标回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // 反转Y坐标
    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

// 窗口大小改变回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// 键盘输入回调函数
void keyboardCallback(unsigned char key, int x, int y) {
    if (key == 27) { // ESC 键
        exit(0);  // 退出程序
    }

    if (key == 'w' || key == 'W') {
        keys['w'] = true;  // 标记按下了 W 键
    }
    if (key == 's' || key == 'S') {
        keys['s'] = true;  // 标记按下了 S 键
    }
    if (key == 'a' || key == 'A') {
        keys['a'] = true;  // 标记按下了 A 键
    }
    if (key == 'd' || key == 'D') {
        keys['d'] = true;  // 标记按下了 D 键
    }
}

// 键盘释放回调函数
void keyboardUpCallback(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') {
        keys['w'] = false;  // 取消标记 W 键被按下
    }
    if (key == 's' || key == 'S') {
        keys['s'] = false;  // 取消标记 S 键被按下
    }
    if (key == 'a' || key == 'A') {
        keys['a'] = false;  // 取消标记 A 键被按下
    }
    if (key == 'd' || key == 'D') {
        keys['d'] = false;  // 取消标记 D 键被按下
    }
}

// 特殊键（如箭头键）输入回调函数
void specialKeyboardCallback(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        specialKeys[GLUT_KEY_UP] = true;
    }
    if (key == GLUT_KEY_DOWN) {
        specialKeys[GLUT_KEY_DOWN] = true;
    }
    if (key == GLUT_KEY_LEFT) {
        specialKeys[GLUT_KEY_LEFT] = true;
    }
    if (key == GLUT_KEY_RIGHT) {
        specialKeys[GLUT_KEY_RIGHT] = true;
    }
}

// 特殊键释放回调函数
void specialKeyboardUpCallback(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        specialKeys[GLUT_KEY_UP] = false;
    }
    if (key == GLUT_KEY_DOWN) {
        specialKeys[GLUT_KEY_DOWN] = false;
    }
    if (key == GLUT_KEY_LEFT) {
        specialKeys[GLUT_KEY_LEFT] = false;
    }
    if (key == GLUT_KEY_RIGHT) {
        specialKeys[GLUT_KEY_RIGHT] = false;
    }
}

// 更新相机（控制相机的运动）
void updateCamera(float deltaTime) {
    if (keys['w']) {
        camera.processKeyboard(FORWARD, deltaTime);
    }
    if (keys['s']) {
        camera.processKeyboard(BACKWARD, deltaTime);
    }
    if (keys['a']) {
        camera.processKeyboard(LEFT, deltaTime);
    }
    if (keys['d']) {
        camera.processKeyboard(RIGHT, deltaTime);
    }

    // 处理特殊键（如箭头键）
    if (specialKeys[GLUT_KEY_UP]) {
        // 做你想要的处理，比如控制相机沿 Y 轴正方向移动
    }
    if (specialKeys[GLUT_KEY_DOWN]) {
        // 做你想要的处理，比如控制相机沿 Y 轴负方向移动
    }
    if (specialKeys[GLUT_KEY_LEFT]) {
        // 做你想要的处理
    }
    if (specialKeys[GLUT_KEY_RIGHT]) {
        // 做你想要的处理
    }
}


AUX_RGBImageRec* LoadBMP(const char* Filename)
{
    int width, height, channels;
    unsigned char* data = stbi_load(Filename, &width, &height, &channels, STBI_rgb);

    if (!data) {
        return NULL;
    }

    AUX_RGBImageRec* image = new AUX_RGBImageRec;
    image->sizeX = width;
    image->sizeY = height;
    image->data = new unsigned char[width * height * 3];

    memcpy(image->data, data, width * height * 3);

    stbi_image_free(data);

    return image;
}

int LoadGLTextures()
{
    int Status = FALSE;
    AUX_RGBImageRec* TextureImage[2];//用于装位图资源的数组 

    memset(TextureImage, 0, sizeof(void*) * 2);//初始化指针数组

    const char* pictures[] = {// 创建一个位图名称数组
        "Assets/snowball.bmp",
        "Assets/snow.bmp",
    };

    for (int i = 0; i < 2; i++)
    {
        if (TextureImage[i] = LoadBMP(pictures[i]))// 加载位图i成功，修改状态标志变量Status为TRUE
        {
            Status = TRUE;

            glGenTextures(1, &texture[i]);     // 为第i个位图创建纹理
            glBindTexture(GL_TEXTURE_2D, texture[i]);// 将生成的纹理的名称绑定到指定的纹理上
            glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[i]->sizeX, TextureImage[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[i]->data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        if (TextureImage[i])         // 释放位图数组占用的内存空间
        {
            if (TextureImage[i]->data)
            {
                free(TextureImage[i]->data);
            }

            free(TextureImage[i]);
        }
    }
    return Status;
}

void UpdateDown()
{
    x += vx;
    y += vy;
    z += vz;

    /** 更新速度 */
    vy += ay;
}

void DrawParticleDown()
{

    for (int i = 0; i < F.GetNumOfParticle(); ++i)
    {
        /** 获得粒子的所有属性 */
        F.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -6.0f);
        glColor4ub(r, g, b, 255);
        glNormal3f(0.0f, 0.0f, 1.0f);   /**< 定义法线方向 */
        /** 画出粒子 */
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size - 2.8, y - size, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size - 2.8, y + size, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size - 2.8, y + size, z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size - 2.8, y - size, z);
        glEnd();

        /** 更新粒子属性 */
        UpdateDown();
        F.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
    }

    for (int i = 0; i < A.GetNumOfParticle(); ++i)
    {
        /** 获得粒子的所有属性 */
        A.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -6.0f);
        glColor4ub(r, g, b, 255);
        glNormal3f(0.0f, 0.0f, 1.0f);   /**< 定义法线方向 */
        /** 画出粒子 */
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size - 2.1, y - size, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size - 2.1, y + size, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size - 2.1, y + size, z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size - 2.1, y - size, z);
        glEnd();

        /** 更新粒子属性 */
        UpdateDown();
        A.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
    }
    for (int i = 0; i < L.GetNumOfParticle(); ++i)
    {
        /** 获得粒子的所有属性 */
        L.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -6.0f);
        glColor4ub(r, g, b, 255);
        glNormal3f(0.0f, 0.0f, 1.0f);   /**< 定义法线方向 */
        /** 画出粒子 */
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size - 1.25, y - size, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size - 1.25, y + size, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size - 1.25, y + size, z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size - 1.25, y - size, z);
        glEnd();

        /** 更新粒子属性 */
        UpdateDown();
        L.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
    }

    for (int i = 0; i < L.GetNumOfParticle(); ++i)
    {
        /** 获得粒子的所有属性 */
        L.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -6.0f);
        glColor4ub(r, g, b, 255);
        glNormal3f(0.0f, 0.0f, 1.0f);   /**< 定义法线方向 */
        /** 画出粒子 */
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size - 0.6, y - size, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size - 0.6, y + size, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size - 0.6, y + size, z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size - 0.6, y - size, z);
        glEnd();

        /** 更新粒子属性 */
        UpdateDown();
        L.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
    }

    for (int i = 0; i < D.GetNumOfParticle(); ++i)
    {
        /** 获得粒子的所有属性 */
        D.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -6.0f);
        glColor4ub(r, g, b, 255);
        glNormal3f(0.0f, 0.0f, 1.0f);   /**< 定义法线方向 */
        /** 画出粒子 */
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size + 0.2, y - size, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size + 0.2, y + size, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size + 0.2, y + size, z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size + 0.2, y - size, z);
        glEnd();

        /** 更新粒子属性 */
        UpdateDown();
        D.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
    }

    for (int i = 0; i < O.GetNumOfParticle(); ++i)
    {
        /** 获得粒子的所有属性 */
        O.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -6.0f);
        glColor4ub(r, g, b, 255);
        glNormal3f(0.0f, 0.0f, 1.0f);   /**< 定义法线方向 */
        /** 画出粒子 */
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size + 1.25, y - size, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size + 1.25, y + size, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size + 1.25, y + size, z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size + 1.25, y - size, z);
        glEnd();

        /** 更新粒子属性 */
        UpdateDown();
        O.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
    }

    for (int i = 0; i < W.GetNumOfParticle(); ++i)
    {
        /** 获得粒子的所有属性 */
        W.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -6.0f);
        glColor4ub(r, g, b, 255);
        glNormal3f(0.0f, 0.0f, 1.0f);   /**< 定义法线方向 */
        /** 画出粒子 */
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size + 2.4, y - size, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size + 2.4, y + size, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size + 2.4, y + size, z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size + 2.4, y - size, z);
        glEnd();

        /** 更新粒子属性 */
        UpdateDown();
        W.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
    }

    for (int i = 0; i < N.GetNumOfParticle(); ++i)
    {
        /** 获得粒子的所有属性 */
        N.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -6.0f);
        glColor4ub(r, g, b, 255);
        glNormal3f(0.0f, 0.0f, 1.0f);   /**< 定义法线方向 */
        /** 画出粒子 */
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size + 3.7, y - size, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size + 3.7, y + size, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size + 3.7, y + size, z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size + 3.7, y - size, z);
        glEnd();

        /** 更新粒子属性 */
        UpdateDown();
        N.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, fade);
    }



    glutPostRedisplay();
}

void Snow_DrawGround()
{
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -6.0f);
    glRotatef(0, 0.0f, 1.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glColor4ub(255, 255, 255, 255);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-5, -1.0f, 5);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-10, -1.0f, -5);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10, -1.0f, -5);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(5, -1.0f, 5);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glColor4ub(255, 255, 255, 255);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, -1.0f, -2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0, -1.0f, -2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 1.0f, -2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 1.0f, -2);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glColor4ub(255, 255, 255, 255);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(2, -1.0f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3, -1.0f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3, 1.0f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2, 1.0f, -0.5f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glColor4ub(255, 255, 255, 255);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0, -1.0f, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1, -1.0f, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 1.0f, 0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1.0f, 0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glColor4ub(255, 255, 255, 255);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3, -1.0f, 1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-2, -1.0f, 1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2, 1.0f, 1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3, 1.0f, 1);
    glEnd();

    glPopMatrix();
}

float angles = 0.0f;
/** 绘制粒子 */
void DrawParticleSnow()
{
    /** 绑定纹理 */
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    for (int i = 0; i < Snow.GetNumOfParticle(); ++i)
    {
        Snow.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, dec);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -6.0f);
        glColor4ub(r, g, b, 255);
        glNormal3f(0.0f, 0.0f, 1.0f);

        angles += 0.001f;
        if (angles == 360.0f) {
            angles = 0;
        }

        glRotatef(angles, x, y, z);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size, y - size, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size, y + size, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size, y + size, z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size, y - size, z);
        glEnd();

        /** 更新粒子属性 */
        UpdateSnow();
        Snow.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, dec);
    }
    glutPostRedisplay();
}

void displaySnow(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    Snow_DrawGround();
    DrawParticleSnow();

    glFlush();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 获取视图矩阵
    view = camera.getViewMatrix();

    // 渲染天空盒
    glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
    glDepthMask(GL_FALSE);  // 禁用深度写入
    glDepthFunc(GL_LEQUAL); // 允许天空盒遮挡物体
    //skybox.render(skyboxView, projection);
    glDepthMask(GL_TRUE);   // 恢复深度写入
    glDepthFunc(GL_LESS);   // 恢复标准深度测试

    // 渲染草地（不透明物体）
    //grass.render(view, projection);

    // 渲染雪花（透明物体）
    glEnable(GL_DEPTH_TEST); // 启用深度测试

    InitSnow();
    displaySnow();  // 直接调用渲染雪花的函数

    // 渲染岩石（不透明物体）
    glm::mat4 rockModelMatrix = glm::mat4(1.0f);
    rockModelMatrix = glm::translate(rockModelMatrix, glm::vec3(20.0f, 20.0f, 20.0f)); // 设置岩石位置
    rockModelMatrix = glm::scale(rockModelMatrix, glm::vec3(12.0f)); // 调整岩石大小
    //rock.render(view, projection, rockModelMatrix);


    glutSwapBuffers(); // 交换缓冲区
}

void init() {
    glEnable(GL_DEPTH_TEST); // 启用深度测试

    // 初始化投影矩阵
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // 加载天空盒
    std::vector<std::string> faces = {
        "Assets/Skybox/right(3).jpg",
        "Assets/Skybox/left(3).jpg",
        "Assets/Skybox/top(3).jpg",
        "Assets/Skybox/bottom(3).jpg",
        "Assets/Skybox/front(3).jpg",
        "Assets/Skybox/back(3).jpg"
    };
    Skybox skybox(faces);

    // 加载草地
    Grass grass("Assets/grass.png");

    // 初始化雪花
    Snow.Create(1000);
    InitSnow();

    // 加载岩石
    Rock rock("Assets/Rock/stone.obj", "Assets/Rock/stone.mtl");

}

// 主函数
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1600, 1200);
    glutCreateWindow("GLUT Demo");

    init();

    // 注册回调函数
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardCallback);  // 注册普通键盘按下事件
    glutKeyboardUpFunc(keyboardUpCallback);  // 注册普通键盘释放事件
    glutSpecialFunc(specialKeyboardCallback);  // 注册特殊键盘按下事件
    glutSpecialUpFunc(specialKeyboardUpCallback);  // 注册特殊键盘释放事件

    glutMainLoop();  // 进入 GLUT 主循环
    return 0;
}