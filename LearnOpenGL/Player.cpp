#pragma once

#include "Engine.h"
#include "GlobalEntities.h"
#include "Player.h"

#include <algorithm>

//void Player::calculateDirection()
//{
//    if (GE::camera.getPosition() == GE::player.m_positionLastFrame) {
//        GE::player.m_direction = glm::vec3(0.0f);
//    }
//    else {
//        GE::player.m_direction = glm::normalize(GE::camera.getPosition() - GE::player.m_positionLastFrame);
//        GE::player.m_positionLastFrame = GE::camera.getPosition();
//    }
//    std::println("player.m_direction: {}, {}, {}", GE::player.m_direction.x, GE::player.m_direction.y, GE::player.m_direction.z);
//}

void Player::initMovement(CameraMovement direction)
{
    // FORWARD BACKWARD
    if (direction == CameraMovement::FORWARD) {
        m_acceleration.x += GE::camera.getFront().x * m_maxAcceleration;
        m_acceleration.z += GE::camera.getFront().z * m_maxAcceleration;
    }
    if (direction == CameraMovement::BACKWARD) {
        m_acceleration.x -= GE::camera.getFront().x * m_maxAcceleration;
        m_acceleration.z -= GE::camera.getFront().z * m_maxAcceleration;
    }
    if (direction == CameraMovement::FORWARDBACKWARD) {
        m_acceleration.z = 0.0f;
        return;
    }

    // LEFT RIGHT
    if (direction == CameraMovement::LEFT) {
        m_acceleration.x -= GE::camera.getRight().x * m_maxAcceleration;
        m_acceleration.z -= GE::camera.getRight().z * m_maxAcceleration;
    }
    if (direction == CameraMovement::RIGHT) {
        m_acceleration.x += GE::camera.getRight().x * m_maxAcceleration;
        m_acceleration.z += GE::camera.getRight().z * m_maxAcceleration;
    }
    if (direction == CameraMovement::LEFTRIGHT) {
        m_acceleration.x = 0.0f;
        return;
    }

    // UP DOWN
    if (direction == CameraMovement::UP) {
        m_acceleration.y += m_maxAcceleration;
    }
    if (direction == CameraMovement::DOWN) {
        m_acceleration.y -= m_maxAcceleration;
    }
    if (direction == CameraMovement::UPDOWN) {
        m_acceleration.y = 0.0f;
        m_Speed.y = 0.0f; // TODO no deceleration applied
        return;
    }
}

void Player::initJump()
{
    if (!m_isJumping) { // TODO + check if touching ground
        m_acceleration.y += 99999999.0f;
        m_isJumping = true;
        GE::camera.setPositionY(1.5f); // TODO, anders blijf je vallen als je onder de Floor jumpt
    }
}

void Player::limitAcceleration() {
    // correct both axis in same ratio if one succeeds the max
    // X
    if (m_acceleration.x > m_maxAcceleration) {
        m_correction = m_maxAcceleration / m_acceleration.x;
        m_acceleration.x = m_maxAcceleration;
        m_acceleration.z *= m_correction;
    }
    if (m_acceleration.x < -m_maxAcceleration) {
        m_correction = -m_maxAcceleration / m_acceleration.x;
        m_acceleration.x = -m_maxAcceleration;
        m_acceleration.z *= m_correction;
    }
    // Y
    if (m_acceleration.y > m_maxJumpAcceleration) {
        if (m_isJumping)
            m_acceleration.y = m_maxJumpAcceleration;
        if (!m_isJumping)
            m_acceleration.y = m_maxJumpAcceleration;
        //if (m_isFloating) TODO
    }
    if (m_acceleration.y < -m_maxJumpAcceleration) {
        if (m_isJumping)
            m_acceleration.y = -m_maxJumpAcceleration;
        if (!m_isJumping)
            m_acceleration.y = -m_maxJumpAcceleration;
        //if (m_isFloating) TODO
    }
    // Z
    if (m_acceleration.z > m_maxAcceleration) {
        m_correction = m_maxAcceleration / m_acceleration.z;
        m_acceleration.z = m_maxAcceleration;
        m_acceleration.x *= m_correction;
    }
    if (m_acceleration.z < -m_maxAcceleration) {
        m_correction = -m_maxAcceleration / m_acceleration.z;
        m_acceleration.z = -m_maxAcceleration;
        m_acceleration.x *= m_correction;
    }

    std::println("initMovement m_acceleration: {}, {}, {}", GE::player.m_acceleration.x, GE::player.m_acceleration.y, GE::player.m_acceleration.z);
}

void Player::handleMovement()
{
    //m_Speed.y += ((G::gravity + m_acceleration.y) * m_Speed.y + 0.0f) * -m_Speed.y * Engine::physicsFrameTime; // * 0.5f;

    m_SpeedLastFrame = m_Speed;

    // Determine new speed with current acceleration force
    m_Speed.x += m_acceleration.x * Engine::physicsFrameTime * 0.5f;
    m_Speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime; // * 0.5f;
    m_Speed.z += m_acceleration.z * Engine::physicsFrameTime * 0.5f;
    //GE::camera.setPositionY(GE::camera.getPosition().y + ((m_Speed.y + m_SpeedLastFrame.y) / 2.0f) * Engine::physicsFrameTime);
    GE::camera.setPosition(GE::camera.getPosition() + ((m_Speed + m_SpeedLastFrame) * 0.5f) * Engine::physicsFrameTime);
    m_Speed.x += m_acceleration.x * Engine::physicsFrameTime * 0.5f;
    m_Speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime; // * 0.5f;
    m_Speed.z += m_acceleration.z * Engine::physicsFrameTime * 0.5f;


    // Apply drag or friction to speed (slows you down when no input is given)
    if (!m_isJumping) {
        m_Speed.x *= m_dryFriction * Engine::physicsFrameTime;
        m_Speed.y *= m_dryFriction * Engine::physicsFrameTime; // TODO dit is alleen true als je omhoog/omlaag LOOPT?
        m_Speed.z *= m_dryFriction * Engine::physicsFrameTime;
    }

    // TODO als je springt moet snelheid vooruit omlaag hoe groter de hoek, en mag niet negatief worden

    if (m_isJumping) {
        // X
        //m_Speed.x *= m_aeroDrag * Engine::physicsFrameTime; // moet icm z-as worden gelimiteerd
        //if (m_Speed.x < 0.0f)
        //    m_Speed.x = 0.0f;
        // Y
        if (m_Speed.y > 0.0f) // zorgt voor stotters?
            m_Speed.y *= m_aeroDrag * Engine::physicsFrameTime;
        else 
            m_Speed.y /= m_aeroDrag * Engine::physicsFrameTime; //  gravity should primary cause the negative acceleration
        // Z
        //m_Speed.z *= m_aeroDrag * Engine::physicsFrameTime; // moet icm x-as worden gelimiteerd
        //if (m_Speed.z < 0.0f)
        //    m_Speed.z = 0.0f;
    }

    static bool jumpStarted = false;
    if (m_isJumping && GE::camera.getPosition().y > 1.51f) {
        jumpStarted = true;
    }

    if (jumpStarted && m_isJumping && GE::camera.getPosition().y <= 1.5f) { // landed on Floor // TODO add real collision check
        m_acceleration.y = -G::gravity;
        GE::camera.setPositionY(1.5f);
        m_Speed.y = 0.0f;
        Engine::extrapolationResultPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        m_isJumping = false;
        jumpStarted = false;
    }

    // Apply some SlowdownFactor to acceleration (for momentum)
    // X
    if (m_acceleration.x != 0.0f) {
        m_acceleration.x *= m_accelerationSlowdownFactor * Engine::physicsFrameTime;
        if (m_acceleration.x < abs(0.001f))
            m_acceleration.x = 0.0f;
    }
    // Y
    if (m_isJumping || G::gravity + m_acceleration.y != 0.0f) {
        m_acceleration.y *= m_accelerationSlowdownFactor * Engine::physicsFrameTime;
        if (m_acceleration.y < abs(0.001f))
            m_acceleration.y = -G::gravity;
    }
    // Z
    if (m_acceleration.z != 0.0f) {
        m_acceleration.z *= m_accelerationSlowdownFactor * Engine::physicsFrameTime;
        if (m_acceleration.z < abs(0.001f))
            m_acceleration.z = 0.0f;
    }

    std::println("handleMovement m_acceleration: {}, {}, {}", GE::player.m_acceleration.x, GE::player.m_acceleration.y, GE::player.m_acceleration.z);
}

void Player::limitSpeed() 
{
    // Limit speed
    //if (m_Speed.x > maxSpeed && (m_Speed.z > maxSpeed) ? TODO ook negatief
    // X
    if (m_Speed.x > m_maxCurrentSpeed) {
        m_correction = m_maxCurrentSpeed / m_Speed.x;
        m_Speed.x = m_maxCurrentSpeed;
        m_Speed.z *= m_correction;
    }
    if (m_Speed.x < -m_maxCurrentSpeed) {
        m_correction = -m_maxCurrentSpeed / m_Speed.x;
        m_Speed.x = -m_maxCurrentSpeed;
        m_Speed.z *= m_correction;
    }
    if (m_Speed.x > -0.001f && m_Speed.x < 0.001f)
        m_Speed.x = 0.0f;
    // Y
    if (m_Speed.y > m_maxJumpSpeed) {
        m_Speed.y = m_maxJumpSpeed;
    }
    if (m_Speed.y < -m_maxJumpSpeed) {
        m_Speed.y = -m_maxJumpSpeed;
    }
    if (m_Speed.y > -0.001f && m_Speed.y < 0.001f)
        m_Speed.y = 0.0f;
    // Z
    if (m_Speed.z > m_maxCurrentSpeed) {
        m_correction = m_maxCurrentSpeed / m_Speed.z;
        m_Speed.z = m_maxCurrentSpeed;
        m_Speed.x *= m_correction;
    }
    if (m_Speed.z < -m_maxCurrentSpeed) {
        m_correction = -m_maxCurrentSpeed / m_Speed.z;
        m_Speed.z = -m_maxCurrentSpeed;
        m_Speed.x *= m_correction;
    }
    if (m_Speed.z > -0.001f && m_Speed.z < 0.001f)
        m_Speed.z = 0.0f;

    std::println("player.m_Speed: {}, {}, {}", GE::player.m_Speed.x, GE::player.m_Speed.y, GE::player.m_Speed.z);
}

void Player::handleJump()
{
    //if (m_isJumping) {
    //    //m_SpeedLastFrame = m_Speed;
    //    //m_Speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime * 0.5f;
    //    //GE::camera.setPositionY(GE::camera.getPosition().y + ((m_Speed.y + m_SpeedLastFrame.y) / 2) * Engine::physicsFrameTime);
    //    //m_Speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime * 0.5f;

    //    if (m_isJumping && GE::camera.getPosition().y <= 1.5f) { // landed on Floor // TODO add real collision check
    //        m_acceleration.y = -G::gravity;
    //        GE::camera.setPositionY(1.5f);
    //        m_Speed.y = 0.0f;
    //        Engine::extrapolationResultPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    //        m_isJumping = false;
    //    }

    //    if (G::gravity + m_acceleration.y != 0.0f)
    //        m_acceleration.y *= m_drag;
    //}
    //std::println("yVelocity: {}", m_ySpeed);
    //std::println("Netto acceleration: {}", (G::gravity + m_acceleration));
}

void Player::handleJumpextrapolation()
{
    if (m_isJumping) {
        //float m_ySpeedforextrapolation{ m_ySpeed + (G::gravity + m_acceleration) * Engine::extrapolationFactor * Engine::physicsFrameTime * 0.5f }; // needed for extra accuracy?
        Engine::extrapolationResultPosition.y = m_Speed.y * Engine::extrapolationFactor * Engine::physicsFrameTime;
    }
    std::println("extrapolationResultPositionY: {}", Engine::extrapolationResultPosition.y);
}

AABB Player::getTAABB()
{
    // oriented bounding boxes needed
    // Player dimensions:
    // x = 50 cm width
    // y = 2 mtr height, eyes are at 1.8 mtr and 5 cm from front
    // z = 20 cm depth

    //x.m_vecMax.x = GE::camera.getPosition().x + 0.25f; // right
    //x.m_vecMax.y = GE::camera.getPosition().y + 0.20f; // top
    //x.m_vecMax.z = GE::camera.getPosition().z + 0.15f; // back

    //x.m_vecMin.x = GE::camera.getPosition().x - 0.25f; // left
    //x.m_vecMin.y = GE::camera.getPosition().y - 1.80f; // bottom
    //x.m_vecMin.z = GE::camera.getPosition().z - 0.05f; // front

    // Player dimensions:
    // x = 40 cm width
    // y = 2 mtr height, eyes are at 1.8 mtr
    // z = 40 cm depth

    AABB x{};

    x.m_vecMax.x = GE::camera.getPosition().x + 0.2f; // right
    x.m_vecMax.y = GE::camera.getPosition().y + 0.2f; // top
    x.m_vecMax.z = GE::camera.getPosition().z + 0.2f; // back

    x.m_vecMin.x = GE::camera.getPosition().x - 0.2f; // left
    x.m_vecMin.y = GE::camera.getPosition().y - 1.8f; // bottom
    x.m_vecMin.z = GE::camera.getPosition().z - 0.2f; // front

    return x;
}

//Engine::TAABB Player::getTAABB(glm::vec3 position, glm::vec3 dimensions)
//{
//      // TODO
//
//    TAABB x{};
//
//    x.m_vecMax.x = GE::camera.getPosition().x + 0.25f; // right
//    x.m_vecMax.y = GE::camera.getPosition().y + 0.25f; // top
//    x.m_vecMax.z = GE::camera.getPosition().z + 0.15f; // back
//
//    x.m_vecMin.x = GE::camera.getPosition().x - 0.25f; // left
//    x.m_vecMin.y = GE::camera.getPosition().y - 1.75f; // bottom
//    x.m_vecMin.z = GE::camera.getPosition().z - 0.05f; // front
//
//    return x;
//}

//void Player::handleMovement() 
// v0.3.05
//{
//    //m_Speed.y += ((G::gravity + m_acceleration.y) * m_Speed.y + 0.0f) * -m_Speed.y * Engine::physicsFrameTime; // * 0.5f;
//
//    m_SpeedLastFrame = m_Speed;
//    m_Speed.x += m_acceleration.x * Engine::physicsFrameTime * 0.5f;
//    m_Speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime; // * 0.5f;
//    m_Speed.z += m_acceleration.z * Engine::physicsFrameTime * 0.5f;
//    //GE::camera.setPositionY(GE::camera.getPosition().y + ((m_Speed.y + m_SpeedLastFrame.y) / 2.0f) * Engine::physicsFrameTime);
//    GE::camera.setPosition(GE::camera.getPosition() + ((m_Speed + m_SpeedLastFrame) * 0.5f) * Engine::physicsFrameTime);
//    m_Speed.x += m_acceleration.x * Engine::physicsFrameTime * 0.5f;
//    m_Speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime; // * 0.5f;
//    m_Speed.z += m_acceleration.z * Engine::physicsFrameTime * 0.5f;
//
//    static bool jumpStarted = false;
//    if (m_isJumping && GE::camera.getPosition().y > 1.51f) {
//        jumpStarted = true;
//    }
//
//    if (jumpStarted && m_isJumping && GE::camera.getPosition().y <= 1.5f) { // landed on Floor // TODO add real collision check
//        m_acceleration.y = -G::gravity;
//        GE::camera.setPositionY(1.5f);
//        m_Speed.y = 0.0f;
//        Engine::extrapolationResultPosition = glm::vec3(0.0f, 0.0f, 0.0f);
//        m_isJumping = false;
//        jumpStarted = false;
//    }
//
//    // Apply drag or friction to acceleration (for momentum?)
//    // X
//    if (m_acceleration.x != 0.0f)
//        m_acceleration.x *= dryFriction * Engine::physicsFrameTime;
//    // Y
//    if (m_isJumping || G::gravity + m_acceleration.y != 0.0f)
//        m_acceleration.y *= m_aeroDrag * Engine::physicsFrameTime;
//    if (m_acceleration.y >= -0.001f && m_acceleration.y <= 0.001f)
//        m_acceleration.y = 9.81f;
//    // Z
//    if (m_acceleration.z != 0.0f)
//        m_acceleration.z *= dryFriction * Engine::physicsFrameTime;
//
//    std::println("handleMovement m_acceleration: {}, {}, {}", GE::player.m_acceleration.x, GE::player.m_acceleration.y, GE::player.m_acceleration.z);
//
//    // Apply drag or friction to speed (slows you down when no input is given)
//    m_Speed.x *= dryFriction * Engine::physicsFrameTime;
//    if (!m_isJumping) // TODO dit is alleen true als je omhoog LOOPT?
//        m_Speed.y = (m_Speed.y) * dryFriction * Engine::physicsFrameTime;
//    m_Speed.z *= dryFriction * Engine::physicsFrameTime;
//}