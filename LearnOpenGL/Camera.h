#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

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
    Camera(float aspectRatio, glm::vec3 m_position1 = glm::vec3(0.0f, 0.0f, 0.0f));

    const float getFov() const;
    const float getNearPlane() const;
    const float getFarPlane() const;

    void fakeGravity(GLfloat deltaTime);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    const glm::mat4 GetViewMatrix();
    const glm::mat4 getProjectionMatrix() { return m_projection; };
    //const glm::vec3 getFront() { return m_front; };       // werken niet goed!!!want const?
    //const glm::vec3 getPosition() { return m_position; }; // werken niet goed!!!
    void setAspectRatio(float x) { m_aspectRatio = x; };
    void recalculateProjectionMatrix() { m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane); };

    void ProcessKeyboard(CameraMovement direction, GLfloat deltaTime); //TODO hoofdletters
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(GLfloat yoffset);

private:
    // camera Attributes
    glm::vec3 m_position{};
    glm::vec3 m_front{ 0.0f, 0.0f, -1.0f }; // m_position + m_front = center = is where you are looking at (direction vector)
    glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_right{};
    const glm::vec3 m_worldup{ 0.0f, 1.0f, 0.0f };
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
};