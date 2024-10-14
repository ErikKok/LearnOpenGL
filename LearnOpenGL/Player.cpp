#pragma once

#include "Engine.h"
#include "GlobalEntities.h"
#include "Player.h"

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
        m_acceleration.x += GE::camera.getFront().x * maxAcceleration;
        m_acceleration.z += GE::camera.getFront().z * maxAcceleration;
    }
    if (direction == CameraMovement::BACKWARD) {
        m_acceleration.x -= GE::camera.getFront().x * maxAcceleration;
        m_acceleration.z -= GE::camera.getFront().z * maxAcceleration;
    }
    if (direction == CameraMovement::FORWARDBACKWARD) {
        m_acceleration.z = 0.0f;
        return;
    }

    // LEFT RIGHT
    if (direction == CameraMovement::LEFT) {
        m_acceleration.x -= GE::camera.getRight().x * maxAcceleration;
        m_acceleration.z -= GE::camera.getRight().z * maxAcceleration;
    }
    if (direction == CameraMovement::RIGHT) {
        m_acceleration.x += GE::camera.getRight().x * maxAcceleration;
        m_acceleration.z += GE::camera.getRight().z * maxAcceleration;
    }
    if (direction == CameraMovement::LEFTRIGHT) {
        m_acceleration.x = 0.0f;
        return;
    }

    // UP DOWN
    if (direction == CameraMovement::UP) {
        m_acceleration.y += maxAcceleration;
    }
    if (direction == CameraMovement::DOWN) {
        m_acceleration.y -= maxAcceleration;
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
    if (m_acceleration.x > maxAcceleration) {
        correction = maxAcceleration / m_acceleration.x;
        m_acceleration.x = maxAcceleration;
        m_acceleration.z *= correction;
    }
    if (m_acceleration.x < -maxAcceleration) {
        correction = -maxAcceleration / m_acceleration.x;
        m_acceleration.x = -maxAcceleration;
        m_acceleration.z *= correction;
    }
    // Y
    if (m_acceleration.y > maxJumpAcceleration) {
        if (m_isJumping)
            m_acceleration.y = maxJumpAcceleration;
        //if (m_isFloating) TODO
    }
    if (m_acceleration.y < -maxJumpAcceleration) {
        if (m_isJumping)
            m_acceleration.y = -maxJumpAcceleration;
        //if (m_isFloating) TODO
    }
    // Z
    if (m_acceleration.z > maxAcceleration) {
        correction = maxAcceleration / m_acceleration.z;
        m_acceleration.z = maxAcceleration;
        m_acceleration.x *= correction;
    }
    if (m_acceleration.z < -maxAcceleration) {
        correction = -maxAcceleration / m_acceleration.z;
        m_acceleration.z = -maxAcceleration;
        m_acceleration.x *= correction;
    }

    std::println("initMovement m_acceleration: {}, {}, {}", GE::player.m_acceleration.x, GE::player.m_acceleration.y, GE::player.m_acceleration.z);
}

void Player::handleMovement()
{
    //m_Speed.y += ((G::gravity + m_acceleration.y) * m_Speed.y + 0.0f) * -m_Speed.y * Engine::physicsFrameTime; // * 0.5f;

    m_SpeedLastFrame = m_Speed;
    m_Speed.x += m_acceleration.x * Engine::physicsFrameTime * 0.5f;
    m_Speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime; // * 0.5f;
    m_Speed.z += m_acceleration.z * Engine::physicsFrameTime * 0.5f;
    //GE::camera.setPositionY(GE::camera.getPosition().y + ((m_Speed.y + m_SpeedLastFrame.y) / 2.0f) * Engine::physicsFrameTime);
    GE::camera.setPosition(GE::camera.getPosition() + ((m_Speed + m_SpeedLastFrame) * 0.5f) * Engine::physicsFrameTime);
    m_Speed.x += m_acceleration.x * Engine::physicsFrameTime * 0.5f;
    m_Speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime; // * 0.5f;
    m_Speed.z += m_acceleration.z * Engine::physicsFrameTime * 0.5f;

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

    // Apply drag or friction to acceleration (for momentum?)
    // X
    if (m_acceleration.x != 0.0f)
        m_acceleration.x *= dryFriction * Engine::physicsFrameTime;
    // Y
    if (m_isJumping || G::gravity + m_acceleration.y != 0.0f)
        m_acceleration.y *= m_aeroDrag * Engine::physicsFrameTime;
    if (m_acceleration.y >= -0.001f && m_acceleration.y <= 0.001f)
        m_acceleration.y = 9.81f;
    // Z
    if (m_acceleration.z != 0.0f)
        m_acceleration.z *= dryFriction * Engine::physicsFrameTime;

    std::println("handleMovement m_acceleration: {}, {}, {}", GE::player.m_acceleration.x, GE::player.m_acceleration.y, GE::player.m_acceleration.z);

    // Apply drag or friction to speed (slows you down when no input is given)
    m_Speed.x *= dryFriction * Engine::physicsFrameTime;
    if (!m_isJumping) // TODO dit is alleen true als je omhoog LOOPT?
        m_Speed.y = (m_Speed.y) * dryFriction * Engine::physicsFrameTime;
    m_Speed.z *= dryFriction * Engine::physicsFrameTime;
}

void Player::limitSpeed() 
{
    // Limit speed
    //if (m_Speed.x > maxSpeed && (m_Speed.z > maxSpeed) ? TODO ook negatief
    // X
    if (m_Speed.x > maxCurrentSpeed) {
        correction = maxCurrentSpeed / m_Speed.x;
        m_Speed.x = maxCurrentSpeed;
        m_Speed.z *= correction;
    }
    if (m_Speed.x < -maxCurrentSpeed) {
        correction = -maxCurrentSpeed / m_Speed.x;
        m_Speed.x = -maxCurrentSpeed;
        m_Speed.z *= correction;
    }
    // Y
    // TODO
    // Z
    if (m_Speed.z > maxCurrentSpeed) {
        correction = maxCurrentSpeed / m_Speed.z;
        m_Speed.z = maxCurrentSpeed;
        m_Speed.x *= correction;
    }
    if (m_Speed.z < -maxCurrentSpeed) {
        correction = -maxCurrentSpeed / m_Speed.z;
        m_Speed.z = -maxCurrentSpeed;
        m_Speed.x *= correction;
    }

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