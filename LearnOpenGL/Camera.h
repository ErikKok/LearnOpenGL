#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
    UP,
    DOWN,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

class Camera
{
public:
    Camera(float aspectRatio, glm::vec3 initPosition = glm::vec3(0.0f, 0.0f, 0.0f));

    const glm::vec3 getPosition() const { return m_position; };
    const glm::vec3 getUp() const { return m_up; };
    const float getNearPlane() const { return m_nearPlane; };
    const float getFarPlane() const { return m_farPlane; };
    const float getFov() const { return m_fov; };
    const float getAspectRatio() const { return m_aspectRatio; };
    const glm::mat4 getProjectionMatrix() const { return m_projection; };
    const glm::mat4 getViewMatrix(glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f)) const;
    const glm::mat4 getReverseViewMatrix() const; // not a rearviewmirror, just looking backwards

    void fakeGravity(GLfloat deltaTime);
    void setAspectRatio(float x);

    void processKeyboard(CameraMovement direction);
    void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    void processMouseScroll(GLfloat yoffset);

private:
    // camera Attributes
    glm::vec3 m_position{};
    glm::vec3 m_front{}; // m_position + m_front = center = is where you are looking at (direction vector)
    glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_right{};
    const glm::vec3 m_worldup{ 0.0f, 1.0f, 0.0f }; // default m_up value
    const GLfloat m_nearPlane{ 0.1f };
    const GLfloat m_farPlane{ 100.0f };
    // euler Angles
    GLfloat m_yaw{ -90.0f };
    GLfloat m_pitch{ 0.0f };
    // camera Options
    GLfloat m_movementSpeed{ 2.5f };
    GLfloat m_mouseSensitivity{ 0.035f };
    GLfloat m_fov{ 45.0f };
    float m_aspectRatio{};
    glm::mat4 m_projection{};

    // update m_front, m_right and m_up Vectors using the updated Euler angles
    void updateCameraVectors();
    void recalculateProjectionMatrix();
};