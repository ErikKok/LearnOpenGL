#pragma once

#include "Engine.h"
#include "GlobalEntities.h"
#include "Player.h"

void Player::initJump() {
    if (m_isJumping == false) { // TODO + check if touching ground
        m_acceleration = 200.0f;
        m_isJumping = true;
    }
}

void Player::handleJump() {
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

void Player::handleJumpextrapolation() {
    if (m_isJumping == true) {
        //float m_ySpeedforextrapolation{ m_ySpeed + (G::gravity + m_acceleration) * Engine::extrapolationFactor * Engine::physicsFrameTime * 0.5f }; // needed for extra accuracy?
        Engine::extrapolationResultPosition.y = m_ySpeed * Engine::extrapolationFactor * Engine::physicsFrameTime;
    }
    std::println("extrapolationResultPositionY: {}", Engine::extrapolationResultPosition.y);
}