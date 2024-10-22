#pragma once
#include "Player.h"

#include "Camera.h"
#include "Engine.h" // for struct AABB

void Player::calculateForwardSpeed()
{
    m_forwardSpeed = glm::dot(m_speed, G::camera->getFront());
}

//void Player::calculateRightSpeed()
//{
//    m_rightSpeed = glm::dot(m_speed, G::cam->getRight());
//}

void Player::initMovement(PlayerMovement direction)
{
    // TODO switch maken?

    if (m_isAirborne) {
        // FORWARD BACKWARD
        //calculateForwardSpeed();
        if (direction == PlayerMovement::forward  && m_forwardSpeed < 0.0f || // player moves backward and inputs forward
            direction == PlayerMovement::backward && m_forwardSpeed > 0.0f || // player moves forwards and inputs backward
            direction == PlayerMovement::forwardbackward) {
            m_speed.x *= m_airborneDecelerationFactor * Engine::physicsFrameTime;
            m_speed.z *= m_airborneDecelerationFactor * Engine::physicsFrameTime;
            return;
        }
        if (direction == PlayerMovement::updown) {
            m_acceleration.y = -G::gravity;
            m_speed.y = 0.0f;
            return;
        }

        // LEFT RIGHT
        if (direction == PlayerMovement::left) {
            m_acceleration.x += G::camera->getRight().x * -m_maxJumpSidewaysAcceleration;
            m_acceleration.z += G::camera->getRight().z * -m_maxJumpSidewaysAcceleration;
            return;
        }
        if (direction == PlayerMovement::right) {
            m_acceleration.x += G::camera->getRight().x * m_maxJumpSidewaysAcceleration;
            m_acceleration.z += G::camera->getRight().z * m_maxJumpSidewaysAcceleration;
            return;
        }
        if (direction == PlayerMovement::leftright) {
            m_speed.x *= m_airborneDecelerationFactor * Engine::physicsFrameTime;
            m_speed.z *= m_airborneDecelerationFactor * Engine::physicsFrameTime;
            return;
        }

        return;
    }

    if (!m_isAirborne) {
        // FORWARD BACKWARD
        if (direction == PlayerMovement::forward) {
            m_acceleration.x += G::camera->getFront().x * m_WalkAcceleration;
            m_acceleration.z += G::camera->getFront().z * m_WalkAcceleration;
            return;
        }
        if (direction == PlayerMovement::backward) {
            m_acceleration.x += G::camera->getFront().x * -m_WalkAcceleration;
            m_acceleration.z += G::camera->getFront().z * -m_WalkAcceleration;
            return;
        }
        //if (direction == PlayerMovement::forwardbackward)
        //    //m_acceleration.z = 0.0f;
        //    return;

        // LEFT RIGHT
        if (direction == PlayerMovement::left) {
            m_acceleration.x += G::camera->getRight().x * -m_StrafeAcceleration;
            m_acceleration.z += G::camera->getRight().z * -m_StrafeAcceleration;
            return;
        }
        if (direction == PlayerMovement::right) {
            m_acceleration.x += G::camera->getRight().x * m_StrafeAcceleration;
            m_acceleration.z += G::camera->getRight().z * m_StrafeAcceleration;
            return;
        }
        //if (direction == PlayerMovement::leftright)
        //    //m_acceleration.x = 0.0f;
        //    return;

        // UP DOWN
        if (direction == PlayerMovement::up) {
            m_acceleration.y += m_maxJumpAcceleration;
            return;
        }
        if (direction == PlayerMovement::down) {
            m_acceleration.y += -m_maxJumpAcceleration;
            return;
        }
        //if (direction == PlayerMovement::updown)
        //    //m_acceleration.y = -G::gravity;
        //    return;

        // JUMP
        if (direction == PlayerMovement::jump) {
            m_acceleration.y = m_jumpAcceleration;
            m_isAirborne = true;
            if (G::camera->getPosition().y < 1.5f)
                G::camera->setPositionY(1.5f); // TODO, anders blijf je vallen als je onder de Floor jumpt
            return;
        }
    }
}

void Player::limitAcceleration() {
    // Y - first because of potentially early return
    if (m_acceleration.y > m_maxJumpAcceleration) {
        if (m_isAirborne)
            m_acceleration.y = m_maxJumpAcceleration;
        //if (!m_isAirborne) // TODO going up or down a slope
        //    m_acceleration.y = m_maxJumpAcceleration;
    }
    if (m_acceleration.y < -m_maxJumpAcceleration) {
        if (m_isAirborne)
            m_acceleration.y = -m_maxJumpAcceleration;
        //if (!m_isAirborne) // TODO going up or down a slope
        //    m_acceleration.y = -m_maxJumpAcceleration;
    }

    // Both axis hits the maxAcceleration simultaneously
    if (m_acceleration.x > m_maxAcceleration && m_acceleration.z > m_maxAcceleration) {
        m_acceleration.x = m_maxAcceleration;
        m_acceleration.z = m_maxAcceleration;
        return;
    }
    if (m_acceleration.x > m_maxAcceleration && m_acceleration.z < -m_maxAcceleration) {
        m_acceleration.x = m_maxAcceleration;
        m_acceleration.z = -m_maxAcceleration;
        return;
    }
    if (m_acceleration.x < -m_maxAcceleration && m_acceleration.z > m_maxAcceleration) {
        m_acceleration.x = -m_maxAcceleration;
        m_acceleration.z = m_maxAcceleration;
        return;
    }
    if (m_acceleration.x < -m_maxAcceleration && m_acceleration.z < -m_maxAcceleration) {
        m_acceleration.x = -m_maxAcceleration;
        m_acceleration.z = -m_maxAcceleration;
        return;
    }

    float m_correction{ 0.0f };
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
}

void Player::calculateSpeed()
{
    // OLD method
    {
        //m_speedLastFrame = m_speed;
        //m_speed.x += m_acceleration.x * Engine::physicsFrameTime * 0.5f;
        //m_speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime; // * 0.5f; // just feels better not halving y
        //m_speed.z += m_acceleration.z * Engine::physicsFrameTime * 0.5f;

        //limitSpeed();
        //G::cam->setPosition(G::cam->getPosition() + ((m_speed + m_speedLastFrame) * 0.5f) * Engine::physicsFrameTime);

        //m_speed.x += m_acceleration.x * Engine::physicsFrameTime * 0.5f;
        //m_speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime; // * 0.5f; // just feels better not halving y
        //m_speed.z += m_acceleration.z * Engine::physicsFrameTime * 0.5f;
    }

    // Determine new speed with current acceleration forces
    m_speedLastFrame = m_speed;
    m_speed.x += m_acceleration.x * Engine::physicsFrameTime;
    if (!m_isAirborne)
        m_speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime;
    else
        m_speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime * m_gravityBoost;
    m_speed.z += m_acceleration.z * Engine::physicsFrameTime;
    
    // Apply friction
    if (!m_isAirborne) {
        m_speed.x *= m_dryFriction * Engine::physicsFrameTime;
        m_speed.y *= m_dryFriction * Engine::physicsFrameTime;
        m_speed.z *= m_dryFriction * Engine::physicsFrameTime;
    }

    // Apply aeroDrag
    if (m_isAirborne) {
        m_speed.x *= m_aeroDrag * Engine::physicsFrameTime;
        m_speed.y *= m_aeroDrag * Engine::physicsFrameTime;
        m_speed.z *= m_aeroDrag * Engine::physicsFrameTime;
    }

    // https://gamedev.stackexchange.com/questions/15708/how-can-i-implement-gravity
    {
        //acceleration = force(time, position) / mass;
        //time += timestep;
        // 
        //position += timestep * velocity;
        //velocity += timestep * acceleration;
        
        //the velocity Verlet method does it like this:
        // TODO? samenvatting: halve snelheid bij positie, dan acceleratie opnieuw berekenen, dan weer helft bij snelheid alsvast voor de volg frame

        //acceleration = force(time, position) / mass;
        //time += timestep;
        // 
        //position += (velocity + timestep * acceleration / 2) * timestep;
        //newAcceleration = force(time, position) / mass; // ergens in de comments: it seems that they sacrificed some accuracy for extra speed by saving the newAcceleration value computed using the estimated velocity and reusing it as the acceleration for the next timestep. (alleen mass zou kunnen veranderen toch?)
        //velocity += (acceleration + newAcceleration) / 2 * timestep;

        /////
        //Dit is wat ik nu heb:
        //You can fix most of the issues with Euler integration simply by replacing
        //position += velocity * timestep
        //    above with 
        //position += (velocity - acceleration * timestep / 2) * timestep
        //    
        //    (where velocity - acceleration * timestep / 2 is simply the average of the old and new velocities)
    }
}

void Player::limitSpeed() 
{
    // TODO or should friction alone limit the speed?
    
    // Y - first because of potentially early return
    if (m_speed.y > m_maxJumpSpeed) {
        m_speed.y = m_maxJumpSpeed;
    }
    if (m_speed.y < -m_maxJumpSpeed) {
        m_speed.y = -m_maxJumpSpeed;
    }
    if (m_speed.y > -0.001f && m_speed.y < 0.001f)
        m_speed.y = 0.0f;

    // Both axis hits the m_maxCurrentSpeed simultaneously | TODO useful?
    if (m_speed.x > m_maxCurrentSpeed && m_speed.z > m_maxCurrentSpeed) {
        m_speed.x = m_maxCurrentSpeed;
        m_speed.z = m_maxCurrentSpeed;
        return;
    }
    if (m_speed.x < -m_maxCurrentSpeed && m_speed.z > m_maxCurrentSpeed) {
        m_speed.x = -m_maxCurrentSpeed;
        m_speed.z = m_maxCurrentSpeed;
        return;
    }
    if (m_speed.x > m_maxCurrentSpeed && m_speed.z < -m_maxCurrentSpeed) {
        m_speed.x = m_maxCurrentSpeed;
        m_speed.z = -m_maxCurrentSpeed;
        return;
    }
    if (m_speed.x < -m_maxCurrentSpeed && m_speed.z < -m_maxCurrentSpeed) {
        m_speed.x = -m_maxCurrentSpeed;
        m_speed.z = -m_maxCurrentSpeed;
        return;
    }

    float m_correction{ 0.0f };
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
}

void Player::handleJump()
{
    // TODO temp jump stuff
    static bool jumpStarted = false;
    if (m_isAirborne && G::camera->getPosition().y > 1.51f) {
        jumpStarted = true;
    }

    // landed on Floor // TODO add real collision check
    if (jumpStarted && m_isAirborne && G::camera->getPosition().y <= 1.5f || G::camera->getPosition().y <= -200.0f) {
        G::camera->setPositionY(1.5f);
        m_speed.y = 0.0f;
        Engine::extrapolationResultPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        m_isAirborne = false;
        jumpStarted = false;
    }
}

void Player::resetAcceleration()
{
    // Reset Acceleration after being applied each physics tick
    m_acceleration.x = 0.0f;
    if (!m_isAirborne)
        m_acceleration.y = -G::gravity;
    if (m_isAirborne)
        m_acceleration.y = 0.0f; //  -G::gravity;
    m_acceleration.z = 0.0f;
}

AABB Player::getTAABB()
{
    // oriented bounding boxes needed
    // Player dimensions:
    // x = 50 cm width
    // y = 2 mtr height, eyes are at 1.8 mtr and 5 cm from front
    // z = 20 cm depth

    //x.m_vecMax.x = G::cam->getPosition().x + 0.25f; // right
    //x.m_vecMax.y = G::cam->getPosition().y + 0.20f; // top
    //x.m_vecMax.z = G::cam->getPosition().z + 0.15f; // back

    //x.m_vecMin.x = G::cam->getPosition().x - 0.25f; // left
    //x.m_vecMin.y = G::cam->getPosition().y - 1.80f; // bottom
    //x.m_vecMin.z = G::cam->getPosition().z - 0.05f; // front

    // Player dimensions:
    // x = 40 cm width
    // y = 2 mtr height, eyes are at 1.8 mtr
    // z = 40 cm depth

    AABB x{};

    x.m_vecMax.x = G::camera->getPosition().x + 0.2f; // right
    x.m_vecMax.y = G::camera->getPosition().y + 0.2f; // top
    x.m_vecMax.z = G::camera->getPosition().z + 0.2f; // back

    x.m_vecMin.x = G::camera->getPosition().x - 0.2f; // left
    x.m_vecMin.y = G::camera->getPosition().y - 1.8f; // bottom
    x.m_vecMin.z = G::camera->getPosition().z - 0.2f; // front

    return x;
}

AABB Player::getTAABB(glm::vec3& proposedPosition)
{
    AABB x{};

    x.m_vecMax.x = proposedPosition.x + 0.2f; // right
    x.m_vecMax.y = proposedPosition.y + 0.2f; // top
    x.m_vecMax.z = proposedPosition.z + 0.2f; // back

    x.m_vecMin.x = proposedPosition.x - 0.2f; // left
    x.m_vecMin.y = proposedPosition.y - 1.8f; // bottom
    x.m_vecMin.z = proposedPosition.z - 0.2f; // front

    return x;
}

//Engine::TAABB Player::getTAABB(glm::vec3 position, glm::vec3 dimensions)
//{
//      // TODO
//
//    TAABB x{};
//
//    x.m_vecMax.x = G::cam->getPosition().x + 0.25f; // right
//    x.m_vecMax.y = G::cam->getPosition().y + 0.25f; // top
//    x.m_vecMax.z = G::cam->getPosition().z + 0.15f; // back
//
//    x.m_vecMin.x = G::cam->getPosition().x - 0.25f; // left
//    x.m_vecMin.y = G::cam->getPosition().y - 1.75f; // bottom
//    x.m_vecMin.z = G::cam->getPosition().z - 0.05f; // front
//
//    return x;
//}

//void Player::initDirection() {
//    m_positionLastFrame = G::cam->getPosition();
//}
//
//void Player::calculateDirection()
//{
//    //if (G::cam->getPosition() == m_positionLastFrame)
//    //    m_direction = glm::vec3(0.0f);
//    //else {
//        m_direction = glm::normalize(G::cam->getPosition() - m_positionLastFrame);
//        m_positionLastFrame = G::cam->getPosition();
//    //}
// 
//    m_positionLastFrame = G::cam->getPosition();
//
//    //std::println("player.m_direction: {}, {}, {}", m_direction.x, m_direction.y, m_direction.z);
//}