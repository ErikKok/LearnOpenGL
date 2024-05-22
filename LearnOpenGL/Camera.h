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
    Camera(float aspectRatio, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

    const glm::vec3 getPosition() const { return m_position; };
    const glm::vec3 getUp() const { return m_up; };
    const float getNearPlane() const { return m_nearPlane; };
    void setNearPlane(float x) { m_nearPlane = x; calculateProjectionMatrixPerspective(); };
    const float getFarPlane() const { return m_farPlane; };
    void setFarPlane(float x) { m_farPlane = x; calculateProjectionMatrixPerspective(); };
    const float getAspectRatio() const { return m_aspectRatio; };
    void setAspectRatio(float x) { m_aspectRatio = x; calculateProjectionMatrixPerspective(); };
    const float getFov() const { return m_fov; };
    void setFov(float x) { m_fov = x; calculateProjectionMatrixPerspective(); };

    const glm::mat4 calculateViewMatrix();
    const glm::mat4 getViewMatrix() const { return m_viewMatrix; };
    void setViewMatrix(glm::mat4 mat4) { m_viewMatrix = mat4; };

    void calculateProjectionMatrixPerspective();
    void calculateProjectionMatrixOrthographic();
    const glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; };

    void calculateViewProjectionMatrix() { m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix; };
    const glm::mat4 getViewProjectionMatrix() const { return m_viewProjectionMatrix; };

    void fakeGravity(GLfloat deltaTime);

    void processKeyboard(CameraMovement direction);
    void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    void processMouseScroll(GLfloat yoffset);

private:
    // camera Attributes
    glm::vec3 m_position{};
    glm::vec3 m_front{};
    glm::vec3 m_up{};
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
    float m_aspectRatio{};
    glm::mat4 m_viewMatrix{};
    glm::mat4 m_projectionMatrix{};
    glm::mat4 m_viewProjectionMatrix{};
     
    // recalculate projectionMatrix after changing
    float m_leftOrtho{ -20.0f };				// Used for Orthographic ProjectionMatrix
    float m_rightOrtho{ 20.0f };				// Used for Orthographic ProjectionMatrix
    float m_bottomOrtho{ -20.0f };			// Used for Orthographic ProjectionMatrix
    float m_topOrtho{ 20.0f };				// Used for Orthographic ProjectionMatrix

    // update m_front, m_right and m_up Vectors using the updated Euler angles
    void updateCameraVectors();
};

//class OrthographicCamera : public Camera{
//public:
//     void calculateProjectionMatrixOrthographic() { m_projectionMatrix = glm::ortho(m_left, m_right, m_bottom, m_top, -m_nearPlane, m_farPlane); };
//    void recalculateProjectionMatrix() override;
//
//private:
//    float m_left{ -20.0f };
//    float m_right{ 20.0f };
//    float m_bottom{ -20.0f };
//    float m_top{ 20.0f };
//};
//
//class PerspectiveCamera : public Camera {
//public:
//     void calculateProjectionMatrixPerspective(Texture& texture) { m_projectionMatrix = glm::perspective(glm::radians(m_fov), static_cast<float>(texture.getWidth() / texture.getHeight()), m_nearPlane, m_farPlane); };
//    void recalculateProjectionMatrix() override;
// 
//    const float getFov() const { return m_fov; };
//
//private:
//
//};