#pragma once

#include <vector>

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
    //static inline std::vector<Camera> cameras;
    //Camera::cameras.emplace_back(1.0f, glm::vec3(0.0f, 1.5f, 15.0f) + glm::vec3(0.4f, -0.5f, -0.3f));
    //Camera::cameras[1].whatever

    Camera(float aspectRatio, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

    const glm::vec3 getPosition() const { return m_position; };
    const glm::vec3 getUp() const { return m_up; };
    const float getNearPlane() const { return m_nearPlane; };
    void setNearPlane(float x) { m_nearPlane = x; calculateProjectionMatrix(); };
    const float getFarPlane() const { return m_farPlane; };
    void setFarPlane(float x) { m_farPlane = x; calculateProjectionMatrix(); };
    const float getAspectRatio() const { return m_aspectRatio; };
    void setAspectRatio(float x) { m_aspectRatio = x; calculateProjectionMatrix(); };
    const float getFov() const { return m_fov; };
    void setFov(float x) { m_fov = x; calculateProjectionMatrix(); };

    virtual const void calculateViewMatrix();
    const glm::mat4 getViewMatrix() const { return m_viewMatrix; };
    void setViewMatrix(glm::mat4 mat4) { m_viewMatrix = mat4; m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix; }; // TODO

    const glm::mat4 getViewProjectionMatrix() const { return m_viewProjectionMatrix; };
    virtual void calculateProjectionMatrix() { m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane); m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix; }; // TODO
    const glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; };

    void fakeGravity(GLfloat deltaTime); // TODO

    void processKeyboard(CameraMovement direction);
    void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    void processMouseScroll(GLfloat yoffset);

protected:
    Camera() {};

    // camera Attributes
    glm::vec3 m_position{};
    glm::vec3 m_front{ 0.0f, 0.0f, 0.0f };
    glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_right{};
    const glm::vec3 m_defaultUp{ 0.0f, 1.0f, 0.0f };
    GLfloat m_nearPlane{ 0.1f };
    GLfloat m_farPlane{ 400.0f };
    // euler Angles
    GLfloat m_yaw{ -90.0f };
    GLfloat m_pitch{ 0.0f };
    // camera Options
    GLfloat m_movementSpeed{ 2.5f };
    GLfloat m_mouseSensitivity{ 0.035f };
    GLfloat m_fov{ 45.0f };
    float m_aspectRatio{ 1.0f };
    glm::mat4 m_viewMatrix{};
    glm::mat4 m_projectionMatrix{};
    glm::mat4 m_viewProjectionMatrix{};   

    // update m_front, m_right and m_up Vectors using the updated Euler angles
    void updateCameraVectors();
};

class OrthographicCamera : public Camera{
public:
    OrthographicCamera(glm::vec3 direction, float left, float right, float bottom, float top);

    const void calculateViewMatrix() override;
    void calculateProjectionMatrix() override { m_projectionMatrix = glm::ortho(m_leftOrtho, m_rightOrtho, m_bottomOrtho, m_topOrtho, m_nearPlane, m_farPlane); m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix; };

private:
    glm::vec3 m_direction{};
    // setters for below members should execute calculateProjectionMatrix
    float m_leftOrtho{ -20.0f };
    float m_rightOrtho{ 20.0f };
    float m_bottomOrtho{ -20.0f };
    float m_topOrtho{ 20.0f };
};