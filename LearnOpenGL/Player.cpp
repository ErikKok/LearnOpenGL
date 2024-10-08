#pragma once

#include "Engine.h"
#include "GlobalEntities.h"
#include "Player.h"

void Player::initJump() {
    if (m_isJumping == false) { // + check if touching ground
        m_acceleration = 200.0f * Engine::physicsFrameTimeFactor;
        m_isJumping = true;
    }
}

void Player::handleJump() {
    std::println("deltaTime: {}", G::deltaTime);
    //if (G::deltaTime < 0.0069f * 1.0f)
    //    G::deltaTime = 0.0069f;

    m_ySpeedLastFrame = m_ySpeed;
    m_ySpeed += (G::gravity + m_acceleration) * G::deltaTime * 0.5f;
    GE::camera.setPositionY(GE::camera.getPosition().y + ((m_ySpeed + m_ySpeedLastFrame) / 2) * G::deltaTime);
    m_ySpeed += (G::gravity + m_acceleration) * G::deltaTime * 0.5f;

    if (m_isJumping == true && GE::camera.getPosition().y <= 1.5f) { // landed on Floor
        m_acceleration = -G::gravity;
        GE::camera.setPositionY(1.5f);
        m_ySpeed = 0.0f;
        m_isJumping = false;
    }

    if (G::gravity + m_acceleration != 0.0f)
        m_acceleration *= G::deltaTime * m_drag; // requires fixed fps

    std::println("yVelocity: {}", m_ySpeed);
    std::println("Netto acceleration: {}", (G::gravity + m_acceleration));
}

void Player::handleJumpFixed() { // float interpolationFactor) {

    m_ySpeedLastFrame = m_ySpeed;
    m_ySpeed += (G::gravity + m_acceleration) * Engine::physicsFrameTime * 0.5f;
    GE::camera.setPositionY(GE::camera.getPosition().y + ((m_ySpeed + m_ySpeedLastFrame) / 2) * Engine::physicsFrameTime);
    m_ySpeed += (G::gravity + m_acceleration) * Engine::physicsFrameTime * 0.5f;

    if (m_isJumping == true && GE::camera.getPosition().y <= 1.5f) { // landed on Floor
        m_acceleration = -G::gravity;
        GE::camera.setPositionY(1.5f);
        m_ySpeed = 0.0f;
        m_isJumping = false;
    }

    if (G::gravity + m_acceleration != 0.0f)
        m_acceleration *= m_drag;

    //std::println("yVelocity: {}", m_ySpeed);
    //std::println("Netto acceleration: {}", (G::gravity + m_acceleration));
}

void Player::handleJumpInterpolation() {
    // for rendering only
    //float m_ySpeedforInterpolation{ m_ySpeed + (G::gravity + m_acceleration) * Engine::interpolationFactor * Engine::physicsFrameTime * 0.5f };

    // cost = 6
    //GE::camera.setPositionYforRendering(GE::camera.getPosition().y + ((m_ySpeedforInterpolation + m_ySpeedLastFrame) / 2) * interpolationFactor);

    //cheaper: cost = 4
    //Engine::interpolationResultPositionY = m_ySpeedforInterpolation * Engine::interpolationFactor + m_ySpeed * (1.0f - Engine::interpolationFactor); // TODO double?
    //State state = currentState * alpha + previousState * (1.0 - alpha);

    Engine::interpolationResultPositionY = m_ySpeed * Engine::interpolationFactor * Engine::physicsFrameTime;
    //view_position = position + (speed * interpolation)

    std::println("interpolationResultPositionY: {}", Engine::interpolationResultPositionY);
}

void Player::handleAcceleration() {
    //if (G::gravity + m_acceleration != 0.0f)
    //    m_acceleration *= G::deltaTime * m_drag;

    //std::println("Netto acceleration: {}", (G::gravity + m_acceleration));
}