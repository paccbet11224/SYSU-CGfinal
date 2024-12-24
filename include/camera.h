#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// 定义摄像机移动方向
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float sensitivity;

    glm::vec3 eye;
    glm::vec3 center;

    unsigned int win_width;
    unsigned int win_height;
    Camera();
    Camera(glm::vec3 startPosition)
        : position(startPosition), front(glm::vec3(0.0f, 0.0f, -1.0f)),
        up(glm::vec3(0.0f, 1.0f, 0.0f)), worldUp(up), yaw(-90.0f), pitch(0.0f), sensitivity(0.1f) {
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    virtual void lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
        this->eye = eye;
        this->center = center;
        this->up = up;
    }


    glm::mat4x4 getMatrix() {
        return glm::lookAt(eye, center, up);
    }

    void processMouseMovement(float xoffset, float yoffset) {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch -= yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        updateCameraVectors();
    }

    void processKeyboard(Camera_Movement direction, float delta) {
        float velocity = delta * 2.5f;
        if (direction == FORWARD) position += front * velocity;
        if (direction == BACKWARD) position -= front * velocity;
        if (direction == LEFT) position -= right * velocity;
        if (direction == RIGHT) position += right * velocity;
    }

private:
    void updateCameraVectors() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};

#endif
