#pragma once

#include "Engine.h"
#include "GlobalEntities.h"
#include "Player.h"

void Player::initJump()
{
    if (m_isJumping == false) { // TODO + check if touching ground
        m_acceleration = 200.0f;
        m_isJumping = true;
    }
}

void Player::handleJump()
{
    if (m_isJumping == true) {
        m_ySpeedLastFrame = m_ySpeed;
        m_ySpeed += (G::gravity + m_acceleration) * Engine::physicsFrameTime * 0.5f;
        GE::camera.setPositionY(GE::camera.getPosition().y + ((m_ySpeed + m_ySpeedLastFrame) / 2) * Engine::physicsFrameTime);
        m_ySpeed += (G::gravity + m_acceleration) * Engine::physicsFrameTime * 0.5f;

        if (m_isJumping == true && GE::camera.getPosition().y <= 1.5f) { // landed on Floor // TODO add real collision check
            m_acceleration = -G::gravity;
            GE::camera.setPositionY(1.5f);
            m_ySpeed = 0.0f;
            Engine::extrapolationResultPosition = glm::vec3(0.0f, 0.0f, 0.0f);
            m_isJumping = false;
        }

        if (G::gravity + m_acceleration != 0.0f)
            m_acceleration *= m_drag;
    }
    //std::println("yVelocity: {}", m_ySpeed);
    //std::println("Netto acceleration: {}", (G::gravity + m_acceleration));
}

void Player::handleJumpextrapolation()
{
    if (m_isJumping == true) {
        //float m_ySpeedforextrapolation{ m_ySpeed + (G::gravity + m_acceleration) * Engine::extrapolationFactor * Engine::physicsFrameTime * 0.5f }; // needed for extra accuracy?
        Engine::extrapolationResultPosition.y = m_ySpeed * Engine::extrapolationFactor * Engine::physicsFrameTime;
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