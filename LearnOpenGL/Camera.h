#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FORWARDBACKWARD,    // FORWARD && BACKWARD pressed together
    LEFTRIGHT,          // LEFT && RIGHT pressed together
    UPDOWN,             // UP && DOWN pressed together
    JUMP,               // TODO, hoort hier niet
};

class Camera
{
public:
    Camera(float aspectRatio, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 getPosition() const { return m_position; };
    void setPosition(glm::vec3 x) { m_position = x; calculateViewMatrix(); };
    void setPositionY(float y) { m_position.y = y; calculateViewMatrix(); };
    const glm::vec3& getFront() const { return m_front; };
    void setFront(glm::vec3 x) { m_front = x; };
    const glm::vec3& getUp() const { return m_up; };
    const glm::vec3& getRight() const { return m_right; };
    float getNearPlane() const { return m_nearPlane; };
    void setNearPlane(float x) { m_nearPlane = x; calculateProjectionMatrix(); };
    float getFarPlane() const { return m_farPlane; };
    void setFarPlane(float x) { m_farPlane = x; calculateProjectionMatrix(); };
    void setYawPitch(float yaw, float pitch) { m_yaw = yaw; m_pitch = pitch; updateCameraVectors(); };
    void setMovementSpeed(float x) { m_movementSpeed = x; };
    float getAspectRatio() const { return m_aspectRatio; };
    void setAspectRatio(float x) { m_aspectRatio = x; calculateProjectionMatrix(); };
    float getFov() const { return m_fov; };
    void setFov(float x) { m_fov = x; calculateProjectionMatrix(); };
    bool getOrthographic() const { return m_orthographic; };

    virtual const void calculateViewMatrix();
    const glm::mat4& getViewMatrix() const { return m_viewMatrix; };
    void setViewMatrix(const glm::mat4& mat4) { m_viewMatrix = mat4; m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix; };
    const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; };
    const glm::mat4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; };
    virtual void calculateProjectionMatrix();

    //void processKeyboard(CameraMovement direction);
    void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    void processMouseScroll(GLfloat yoffset);

    // TODO public?
    // update m_front, m_right and m_up Vectors using the updated Euler angles
    void updateCameraVectors();

protected:
    Camera() {};

    glm::vec3 m_position{};
    glm::vec3 m_front{ 0.0f, 0.0f, 0.0f };
    glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_right{};
    const glm::vec3 m_defaultUp{ 0.0f, 1.0f, 0.0f };
    GLfloat m_nearPlane{ 0.1f };
    GLfloat m_farPlane{ 400.0f };
    GLfloat m_yaw{ -90.0f };
    GLfloat m_pitch{ 0.0f };
    GLfloat m_movementSpeed{}; // set in processInput()
    GLfloat m_mouseSensitivity{ 0.035f };
    GLfloat m_fov{ 45.0f }; // (InnerCutOff + OuterCutOff + 15% for attenuation) seems about right
    float m_aspectRatio{ 1.0f };
    bool m_orthographic{ false }; // false == perspective
    glm::mat4 m_viewMatrix{};
    glm::mat4 m_projectionMatrix{};
    glm::mat4 m_viewProjectionMatrix{};


};

class OrthographicCamera : public Camera{
public:
    OrthographicCamera(glm::vec3 direction, float left, float right, float bottom, float top, float nearPlane, float farPlane);

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