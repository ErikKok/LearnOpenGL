#pragma once

#include "Engine.h"
#include "GlobalEntities.h"
#include "Player.h"

#include <algorithm>

void Player::initMovement(CameraMovement direction)
{
    // TODO switch maken?

    // TODO
    // https://forums.unrealengine.com/t/character-movement-direction/30708/4
    float forwardSpeed = glm::dot(m_speed, GE::camera.getFront());
    //float rightSpeed = glm::dot(m_speed, GE::camera.getRight());

    if (m_isAirborne) {
        // FORWARD BACKWARD
        if (direction == CameraMovement::FORWARD  && forwardSpeed < 0.0f || // player moves backward and inputs forward
            direction == CameraMovement::BACKWARD && forwardSpeed > 0.0f || // player moves forwards and inputs backward
            direction == CameraMovement::FORWARDBACKWARD) {
                m_speed.x *= m_airborneDecelerationFactor * Engine::physicsFrameTime;
                m_speed.z *= m_airborneDecelerationFactor * Engine::physicsFrameTime;
                return;
        }

        // LEFT RIGHT
        if (direction == CameraMovement::LEFT) {
            m_acceleration.x += GE::camera.getRight().x * -m_maxAcceleration * 0.005f; // TODO need another maxSpeed for airborne sideways movement
            m_acceleration.z += GE::camera.getRight().z * -m_maxAcceleration * 0.005f;
            return;
        }
        if (direction == CameraMovement::RIGHT) {
            m_acceleration.x += GE::camera.getRight().x * m_maxAcceleration * 0.005f;
            m_acceleration.z += GE::camera.getRight().z * m_maxAcceleration * 0.005f;
            return;
        }
        if (direction == CameraMovement::LEFTRIGHT) {
            m_speed.x *= m_airborneDecelerationFactor * Engine::physicsFrameTime;
            m_speed.z *= m_airborneDecelerationFactor * Engine::physicsFrameTime;
            return;
        }

        return;
    }

    if (!m_isAirborne) {
        // FORWARD BACKWARD
        if (direction == CameraMovement::FORWARD) {
            m_acceleration.x += GE::camera.getFront().x * m_maxAcceleration;
            m_acceleration.z += GE::camera.getFront().z * m_maxAcceleration;
            return;
        }
        if (direction == CameraMovement::BACKWARD) {
            m_acceleration.x += GE::camera.getFront().x * -m_maxAcceleration;
            m_acceleration.z += GE::camera.getFront().z * -m_maxAcceleration;
            return;
        }
        if (direction == CameraMovement::FORWARDBACKWARD) {
            m_acceleration.z = 0.0f;
            return;
        }

        // LEFT RIGHT
        if (direction == CameraMovement::LEFT) {
            m_acceleration.x += GE::camera.getRight().x * -m_maxAcceleration;
            m_acceleration.z += GE::camera.getRight().z * -m_maxAcceleration;
            return;
        }
        if (direction == CameraMovement::RIGHT) {
            m_acceleration.x += GE::camera.getRight().x * m_maxAcceleration;
            m_acceleration.z += GE::camera.getRight().z * m_maxAcceleration;
            return;
        }
        if (direction == CameraMovement::LEFTRIGHT) {
            m_acceleration.x = 0.0f;
            return;
        }

        // UP DOWN
        if (direction == CameraMovement::UP) {
            m_acceleration.y += m_maxJumpAcceleration;
            return;
        }
        if (direction == CameraMovement::DOWN) {
            m_acceleration.y += -m_maxJumpAcceleration;
            return;
        }
        if (direction == CameraMovement::UPDOWN) {
            m_acceleration.y = 0.0f;
            m_speed.y *= m_airborneDecelerationFactor * Engine::physicsFrameTime;
            return;
        }

        // JUMP
        if (direction == CameraMovement::JUMP) {
            m_acceleration.y = 400.0f; // not added (+=)!
            m_isAirborne = true;
            GE::camera.setPositionY(1.5f); // TODO, anders blijf je vallen als je onder de Floor jumpt
            return;
        }
    }
}

void Player::limitAcceleration() {
    // TODO both axis hits the maxAcceleration
    //if (m_acceleration.x > m_maxAcceleration && m_acceleration.z > m_maxAcceleration) {
    //    m_acceleration.x = m_maxAcceleration;
    //    m_acceleration.z = m_maxAcceleration;
    //}
    //if (m_acceleration.x < -m_maxAcceleration && m_acceleration.z > m_maxAcceleration) {
    //    m_acceleration.x = -m_maxAcceleration;
    //    m_acceleration.z = m_maxAcceleration;
    //}
    //if (m_acceleration.x > m_maxAcceleration && m_acceleration.z < -m_maxAcceleration) {
    //    m_acceleration.x = m_maxAcceleration;
    //    m_acceleration.z = -m_maxAcceleration;
    //}
    //if (m_acceleration.x < -m_maxAcceleration && m_acceleration.z < -m_maxAcceleration) {
    //    m_acceleration.x = -m_maxAcceleration;
    //    m_acceleration.z = -m_maxAcceleration;
    //}

    // correct both axis in same ratio if one exceeds the max
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
        if (m_isAirborne)
            m_acceleration.y = m_maxJumpAcceleration;
        if (!m_isAirborne)
            m_acceleration.y = m_maxJumpAcceleration;
    }
    if (m_acceleration.y < -m_maxJumpAcceleration) {
        if (m_isAirborne)
            m_acceleration.y = -m_maxJumpAcceleration;
        if (!m_isAirborne)
            m_acceleration.y = -m_maxJumpAcceleration;
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

    std::println("limitAcceleration m_acceleration: {}, {}, {}", GE::player.m_acceleration.x, GE::player.m_acceleration.y, GE::player.m_acceleration.z);
}

void Player::handleMovement()
{
    m_speedLastFrame = m_speed;
    // Determine new speed with current acceleration force
    //if (m_isAirborne) {
    //    if (m_speed.x > 0.0f)
    //    m_speed.x = std::max(m_speed.x + m_acceleration.x * Engine::physicsFrameTime * 0.5f, 0.0f);
    //    if (m_speed.y > 0.0f)
    //    m_speed.y = std::max(m_speed.y + (G::gravity + m_acceleration.y) * Engine::physicsFrameTime, 0.0f); // * 0.5f; // just feels better not halving y
    //    if (m_speed.z > 0.0f)
    //        m_speed.z = std::max(m_speed.z + m_acceleration.z * Engine::physicsFrameTime * 0.5f, 0.0f);
    //}

    //if (!m_isAirborne) {
        m_speed.x += m_acceleration.x * Engine::physicsFrameTime * 0.5f;
        m_speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime; // * 0.5f; // just feels better not halving y
        m_speed.z += m_acceleration.z * Engine::physicsFrameTime * 0.5f;
    //}
    limitSpeed();
    GE::camera.setPosition(GE::camera.getPosition() + ((m_speed + m_speedLastFrame) * 0.5f) * Engine::physicsFrameTime);

    //if (m_isAirborne) {
    //    m_speed.x = std::max(m_speed.x + m_acceleration.x * Engine::physicsFrameTime * 0.5f, 0.0f);
    //    m_speed.y = std::max(m_speed.y + (G::gravity + m_acceleration.y) * Engine::physicsFrameTime, 0.0f); // * 0.5f; // just feels better not halving y
    //    m_speed.z = std::max(m_speed.z + m_acceleration.z * Engine::physicsFrameTime * 0.5f, 0.0f);
    //}

    //if (!m_isAirborne) {
        m_speed.x += m_acceleration.x * Engine::physicsFrameTime * 0.5f;
        m_speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime; // * 0.5f; // just feels better not halving y
        m_speed.z += m_acceleration.z * Engine::physicsFrameTime * 0.5f;
    //}
    // Reset Acceleration after 1 physics tick
    resetAcceleration();

    std::println("handleMovement m_acceleration: {}, {}, {}", GE::player.m_acceleration.x, GE::player.m_acceleration.y, GE::player.m_acceleration.z);

    // Apply friction to speed
    if (!m_isAirborne) {
        m_speed.x *= m_dryFriction * Engine::physicsFrameTime;
        m_speed.y *= m_dryFriction * Engine::physicsFrameTime; // TODO dit is alleen true als je omhoog/omlaag LOOPT?
        m_speed.z *= m_dryFriction * Engine::physicsFrameTime;
    }

    // TODO temp jump stuff
    static bool jumpStarted = false;
    if (m_isAirborne && GE::camera.getPosition().y > 1.51f) {
        jumpStarted = true;
    }

    if (jumpStarted && m_isAirborne && GE::camera.getPosition().y <= 1.5f) { // landed on Floor // TODO add real collision check
        GE::camera.setPositionY(1.5f);
        m_speed.y = 0.0f;
        Engine::extrapolationResultPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        m_isAirborne = false;
        jumpStarted = false;
    }
}

void Player::limitSpeed() 
{
    // TODO or should friction alone limit the speed?

    // TODO both axis hits m_maxCurrentSpeed | useful?
    if (m_speed.x > m_maxCurrentSpeed && m_speed.z > m_maxCurrentSpeed) {
        m_speed.x = m_maxAcceleration;
        m_speed.z = m_maxAcceleration;
    }
    if (m_speed.x < -m_maxCurrentSpeed && m_speed.z > m_maxCurrentSpeed) {
        m_speed.x = -m_maxAcceleration;
        m_speed.z = m_maxAcceleration;
    }
    if (m_speed.x > m_maxCurrentSpeed && m_speed.z < -m_maxCurrentSpeed) {
        m_speed.x = m_maxAcceleration;
        m_speed.z = -m_maxAcceleration;
    }
    if (m_speed.x < -m_maxCurrentSpeed && m_speed.z < -m_maxCurrentSpeed) {
        m_speed.x = -m_maxAcceleration;
        m_speed.z = -m_maxAcceleration;
    }

    // X
    if (m_speed.x > m_maxCurrentSpeed) {
        m_correction = m_maxCurrentSpeed / m_speed.x;
        m_speed.x = m_maxCurrentSpeed;
        m_speed.z *= m_correction;
    }
    if (m_speed.x < -m_maxCurrentSpeed) {
        m_correction = -m_maxCurrentSpeed / m_speed.x;
        m_speed.x = -m_maxCurrentSpeed;
        m_speed.z *= m_correction;
    }
    if (m_speed.x > -0.001f && m_speed.x < 0.001f)
        m_speed.x = 0.0f;
    // Y
    if (m_speed.y > m_maxJumpSpeed) {
        m_speed.y = m_maxJumpSpeed;
    }
    if (m_speed.y < -m_maxJumpSpeed) {
        m_speed.y = -m_maxJumpSpeed;
    }
    if (m_speed.y > -0.001f && m_speed.y < 0.001f)
        m_speed.y = 0.0f;
    // Z
    if (m_speed.z > m_maxCurrentSpeed) {
        m_correction = m_maxCurrentSpeed / m_speed.z;
        m_speed.z = m_maxCurrentSpeed;
        m_speed.x *= m_correction;
    }
    if (m_speed.z < -m_maxCurrentSpeed) {
        m_correction = -m_maxCurrentSpeed / m_speed.z;
        m_speed.z = -m_maxCurrentSpeed;
        m_speed.x *= m_correction;
    }
    if (m_speed.z > -0.001f && m_speed.z < 0.001f)
        m_speed.z = 0.0f;

    std::println("player.m_Speed: {}, {}, {}", GE::player.m_speed.x, GE::player.m_speed.y, GE::player.m_speed.z);
}

void Player::resetAcceleration()
{
    m_acceleration.x = 0.0f;
    if (!m_isAirborne)
        m_acceleration.y = -G::gravity;
    if (m_isAirborne)
        m_acceleration.y = 0.0f;
    m_acceleration.z = 0.0f;
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

void Player::initDirection() {
    m_positionLastFrame = GE::camera.getPosition();
}

void Player::calculateDirection()
{
    if (GE::camera.getPosition() == m_positionLastFrame)
        m_direction = glm::vec3(0.0f);
    else {
        m_direction = glm::normalize(GE::camera.getPosition() - m_positionLastFrame);
        m_positionLastFrame = GE::camera.getPosition();
    }
 
    m_positionLastFrame = GE::camera.getPosition();

    //std::println("player.m_direction: {}, {}, {}", m_direction.x, m_direction.y, m_direction.z);
}