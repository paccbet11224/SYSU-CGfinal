#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // 初始化GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed!" << std::endl;
        return -1;
    }

    // 创建OpenGL窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW + GLEW Test", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 设置当前上下文
    glfwMakeContextCurrent(window);

    // 初始化GLEW
    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        std::cerr << "GLEW initialization failed: " << glewGetErrorString(glewInitResult) << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "GLEW Initialized! OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // 主渲染循环
    while (!glfwWindowShouldClose(window)) {
        // 清空屏幕为黑色
        glClear(GL_COLOR_BUFFER_BIT);

        // 交换缓冲区
        glfwSwapBuffers(window);

        // 处理事件
        glfwPollEvents();
    }

    // 终止GLFW
    glfwTerminate();
    return 0;
}
