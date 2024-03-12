#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

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
    Camera(glm::vec3 m_position1 = glm::vec3(0.0f, 0.0f, 0.0f));

    const float getFov() const;
    const float getNearPlane() const;
    const float getFarPlane() const;

    void fakeGravity(float deltaTime);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    const glm::mat4 GetViewMatrix() const;

    void ProcessKeyboard(CameraMovement direction, float deltaTime); //TODO hoofdletters
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

private:
    // camera Attributes
    glm::vec3 m_position{ };
    glm::vec3 m_front{ 0.0f, 0.0f, -1.0f };
    glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_right{};
    const glm::vec3 m_worldup{ 0.0f, 1.0f, 0.0f };
    const float m_nearPlane{ 0.1f };
    const float m_farPlane{ 100.0f };
    // euler Angles
    float m_yaw{ -90.0f };
    float m_pitch{ 0.0f };
    // camera Options
    float m_movementSpeed{ 2.5f };
    float m_mouseSensitivity{ 0.035f };
    float m_fov{ 45.0f };

    // update m_front, m_right and m_up Vectors using the updated Euler angles
    void updateCameraVectors();
};