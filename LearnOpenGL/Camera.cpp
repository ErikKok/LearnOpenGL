#pragma once
#include "Camera.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 initPosition)
{
    m_position = initPosition;
    updateCameraVectors();
}

const float Camera::getFov() const { return m_fov; };
const float Camera::getNearPlane() const { return m_nearPlane; };
const float Camera::getFarPlane() const { return m_farPlane; };

void Camera::fakeGravity(float deltaTime) {
    if (m_position.y > 0.15f)
        m_position.y -= 0.2f * m_movementSpeed * deltaTime;
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
const glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{      
    float velocity{ m_movementSpeed * deltaTime }; // TODO deltaTime uit de global ns

    if (direction == CameraMovement::UP)
        m_position.y += 0.5f * velocity;
    if (direction == CameraMovement::DOWN)
        m_position.y -= 0.5f * velocity;

    if (direction == CameraMovement::FORWARD) {
        m_position.x += m_front.x * velocity;
        m_position.z += m_front.z * velocity;
    }
    if (direction == CameraMovement::BACKWARD) {
        m_position.x -= m_front.x * velocity;
        m_position.z -= m_front.z * velocity;
    }
    if (direction == CameraMovement::LEFT) {
        m_position.x -= m_right.x * velocity;
        m_position.z -= m_right.z * velocity;
    }
    if (direction == CameraMovement::RIGHT) {
        m_position.x += m_right.x * velocity;
        m_position.z += m_right.z * velocity;
    }
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    // make sure that when m_pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (m_pitch > 89.99f)
            m_pitch = 89.99f;
        if (m_pitch < -89.99f)
            m_pitch = -89.99f;
    }

    updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
    m_fov -= (float)yoffset * 3;
    if (m_fov < 1.0f)
        m_fov = 1.0f;
    if (m_fov > 45.0f)
        m_fov = 45.0f;
}

// update m_front, m_right and m_up Vectors using the updated Euler angles
void Camera::updateCameraVectors()
{
    glm::vec3 temp{
        temp.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
        temp.y = sin(glm::radians(m_pitch)),
        temp.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
    };
    m_front = glm::normalize(temp);
    m_right = glm::normalize(glm::cross(m_front, m_worldup));  // normalize the vectors, because their length gets closer to 0 the more you look m_up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}