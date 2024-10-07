#pragma once

#include "GlobalEntities.h"
#include "Player.h"

void Player::initJump() {
    if (m_isJumping == false) { // + check if touching ground
        m_acceleration = 666.0f;
        m_isJumping = true;
    }
}

void Player::handleJump() {
    m_yVelocityLastFrame = m_yVelocity;
    m_yVelocity += (G::gravity + m_acceleration) * G::deltaTime;
    GE::camera.setPositionY(GE::camera.getPosition().y + ((m_yVelocity + m_yVelocityLastFrame) / 2) * G::deltaTime);

    if (m_isJumping == true && GE::camera.getPosition().y <= 1.5f) { // landed on Floor
        m_acceleration = -G::gravity;
        GE::camera.setPositionY(1.5f);
        m_yVelocity = 0.0f;
        m_isJumping = false;
    }

    if (G::gravity + m_acceleration != 0.0f)
        m_acceleration *= G::deltaTime * m_drag;

    std::println("yVelocity: {}", m_yVelocity);
    std::println("Netto acceleration: {}", (G::gravity + m_acceleration));
}

void Player::handleAcceleration() {
    if (G::gravity + m_acceleration != 0.0f)
        m_acceleration *= G::deltaTime * m_drag;

    std::println("Netto acceleration: {}", (G::gravity + m_acceleration));
}