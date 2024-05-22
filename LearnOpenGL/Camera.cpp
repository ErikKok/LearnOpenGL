#pragma once
#include "Camera.h"
#include "Global.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float aspectRatio, glm::vec3 position, glm::vec3 front, glm::vec3 up)
    : m_aspectRatio{ aspectRatio }
    , m_position{ position }
    , m_front{ front }
    , m_up{ up }
    , m_defaultUp{ up }
{
    updateCameraVectors();
    calculateViewMatrix();
    calculateProjectionMatrix();
}

OrthographicCamera::OrthographicCamera(glm::vec3 direction, float left, float right, float bottom, float top)
    : m_direction{ direction }
    , m_leftOrtho{ left }
    , m_rightOrtho{ right }
    , m_bottomOrtho{ bottom }
    , m_topOrtho{ top }
{
    updateCameraVectors();
    calculateViewMatrix();
    calculateProjectionMatrix();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
const void Camera::calculateViewMatrix()
{
   m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
   m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

    // TODO 
    // m_position = eye aka target = the position of the camera's viewpoint
    // m_position = center = is where you are looking at (a position)
    // m_position + m_front = center = is where you are looking at (a direction vector)

    // eye is the position of the camera's viewpoint, and center is where you are looking at (a position). If you want to use a direction vector D instead of a center position,
    // you can simply use eye + D as the center position, where D can be a unit vector for example.
 
    // not a rearviewmirror, just looking backwards:
    // glm::mat4 view{ glm::lookAt(m_position, m_position + -m_front, m_up) };

    // view from above, disable mouse
    //glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), m_up);
}

const void OrthographicCamera::calculateViewMatrix()
{
    m_viewMatrix = glm::lookAt(m_direction, { 0.0f, 0.0f, 0.0f }, m_up);
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void Camera::fakeGravity(GLfloat deltaTime) {
    if (m_position.y > 0.15f)
        m_position.y -= 0.2f * m_movementSpeed * deltaTime;
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::processKeyboard(CameraMovement direction)
{      
    float velocity{ m_movementSpeed * Global::deltaTime };

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

    calculateViewMatrix();
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
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
void Camera::processMouseScroll(GLfloat yoffset)
{
    m_fov -= (GLfloat)yoffset * 3;
    if (m_fov < 1.0f)
        m_fov = 1.0f;
    if (m_fov > 45.0f)
        m_fov = 45.0f;
    calculateProjectionMatrix();
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
    m_right = glm::normalize(glm::cross(m_front, m_defaultUp));  // normalize the vectors, because their length gets closer to 0 the more you look m_up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
    
    calculateViewMatrix();

    // TODO
    // This is a very "lazy" way to determine the right and up vectors.It makes a bunch of assumptions, including one that you will never, ever look straight up or down.

    //    Think about it : If we are computing what "Right" is by comparing "Forward" with the world's notion of "up", then what is "Right" supposed to be when "Forward" is pointing straight up or straight down?

    //    A much cleaner way to go about it is to :

    //Create a transformation matrix from Pitch and Yaw
    //    Apply that matrix to all three reference axises.
    //    (Optional)re - orthogonalize the resulting three vectors using cross products.
}