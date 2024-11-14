#pragma once
#include "Player.h"

#include "Camera.h"
#include "Global.h" // for calculateValues
#include "Engine.h" // for struct AABB

void Player::calculateValues()
{
    m_WalkAcceleration = 250.0f * Engine::physicsFrameTime;
    m_StrafeAcceleration = 200.0f * Engine::physicsFrameTime;
    m_AirborneAcceleration = 150.0f * Engine::physicsFrameTime;
    m_jumpAcceleration = 300.0f * Engine::physicsFrameTime;
    m_dryFriction = 92.0f * Engine::physicsFrameTime;
    m_aeroDrag = 99.8f * Engine::physicsFrameTime;
    m_gravityBoost = 350.0f * Engine::physicsFrameTime;
    G::gravity = -9.810f * Engine::physicsFrameTime;
}

void Player::calculateForwardSpeed()
{
    m_forwardSpeed = glm::dot(m_speed, G::camera->getFront());
}

void Player::calculateRightSpeed()
{
    m_rightSpeed = glm::dot(m_speed, G::camera->getRight());
}

void Player::initMovement(PlayerMovement direction)
{
    if (m_isAirborne) {
        // FORWARD BACKWARD
        if (direction == PlayerMovement::forward) { [[likely]]
            m_acceleration.x = G::camera->getFront().x * m_AirborneAcceleration;
            m_acceleration.z = G::camera->getFront().z * m_AirborneAcceleration;
            return;
        }
        if (direction == PlayerMovement::backward) {
            m_acceleration.x = G::camera->getFront().x * -m_AirborneAcceleration;
            m_acceleration.z = G::camera->getFront().z * -m_AirborneAcceleration;
            return;
        }
        if (direction == PlayerMovement::forward && m_forwardSpeed < 0.0f ||  // player moves backward and inputs forward
            direction == PlayerMovement::backward && m_forwardSpeed > 0.0f) { // player moves forwards and inputs backward
            return; // does this ever happen? breakpoint added 6-11-2024
        }

        // LEFT RIGHT
        if (direction == PlayerMovement::left) {
            m_acceleration.x = G::camera->getRight().x * -m_AirborneAcceleration;
            m_acceleration.z = G::camera->getRight().z * -m_AirborneAcceleration;
            return;
        }
        if (direction == PlayerMovement::right) {
            m_acceleration.x = G::camera->getRight().x * m_AirborneAcceleration;
            m_acceleration.z = G::camera->getRight().z * m_AirborneAcceleration;
            return;
        }

        // UP DOWN // for testing
        if (direction == PlayerMovement::updown) { [[unlikely]]
            m_acceleration.y = -G::gravity;
            m_speed.y = 0.0f;
            return;
        }

        return;
    }

    if (!m_isAirborne) {
        // FORWARD BACKWARD
        if (direction == PlayerMovement::forward) { [[likely]]
            m_acceleration.x = G::camera->getFront().x * m_WalkAcceleration;
            m_acceleration.z = G::camera->getFront().z * m_WalkAcceleration;
            return;
        }
        if (direction == PlayerMovement::backward) {
            m_acceleration.x = G::camera->getFront().x * -m_WalkAcceleration;
            m_acceleration.z = G::camera->getFront().z * -m_WalkAcceleration;
            return;
        }

        // LEFT RIGHT
        if (direction == PlayerMovement::left) {
            m_acceleration.x = G::camera->getRight().x * -m_StrafeAcceleration;
            m_acceleration.z = G::camera->getRight().z * -m_StrafeAcceleration;
        }
        if (direction == PlayerMovement::right) {
            m_acceleration.x = G::camera->getRight().x * m_StrafeAcceleration;
            m_acceleration.z = G::camera->getRight().z * m_StrafeAcceleration;
        }

        // UP DOWN
        if (direction == PlayerMovement::up) { [[unlikely]]
            m_acceleration.y = m_jumpAcceleration;
            return;
        }
        if (direction == PlayerMovement::down) { [[unlikely]]
            m_acceleration.y = -m_jumpAcceleration;
            return;
        }

        // JUMP
        if (direction == PlayerMovement::jump) {
            m_acceleration.y = m_jumpAcceleration;
            m_isAirborne = true;
            return;
        }
    }
}

//void Player::limitAcceleration() {
//    // Y - first because of potentially early return
//    if (m_acceleration.y > m_maxJumpAcceleration) { 
//        if (m_isAirborne)
//            m_acceleration.y = m_maxJumpAcceleration;
//        //if (!m_isAirborne) // TODO going up or down a slope
//        //    m_acceleration.y = m_maxJumpAcceleration;
//    }
//    if (m_acceleration.y < -m_maxJumpAcceleration) {
//        if (m_isAirborne)
//            m_acceleration.y = -m_maxJumpAcceleration;
//        //if (!m_isAirborne) // TODO going up or down a slope
//        //    m_acceleration.y = -m_maxJumpAcceleration;
//    }
//
//    // Both axis hits the maxAcceleration simultaneously
//    if (m_acceleration.x > m_maxAcceleration && m_acceleration.z > m_maxAcceleration) {
//        m_acceleration.x = m_maxAcceleration;
//        m_acceleration.z = m_maxAcceleration;
//        return;
//    }
//    if (m_acceleration.x > m_maxAcceleration && m_acceleration.z < -m_maxAcceleration) {
//        m_acceleration.x = m_maxAcceleration;
//        m_acceleration.z = -m_maxAcceleration;
//        return;
//    }
//    if (m_acceleration.x < -m_maxAcceleration && m_acceleration.z > m_maxAcceleration) {
//        m_acceleration.x = -m_maxAcceleration;
//        m_acceleration.z = m_maxAcceleration;
//        return;
//    }
//    if (m_acceleration.x < -m_maxAcceleration && m_acceleration.z < -m_maxAcceleration) {
//        m_acceleration.x = -m_maxAcceleration;
//        m_acceleration.z = -m_maxAcceleration;
//        return;
//    }
//
//    float m_correction{ 0.0f };
//    // X
//    if (m_acceleration.x > m_maxAcceleration) {
//        m_correction = m_maxAcceleration / m_acceleration.x;
//        m_acceleration.x = m_maxAcceleration;
//        m_acceleration.z *= m_correction;
//    }
//    if (m_acceleration.x < -m_maxAcceleration) {
//        m_correction = -m_maxAcceleration / m_acceleration.x;
//        m_acceleration.x = -m_maxAcceleration;
//        m_acceleration.z *= m_correction;
//    }
//
//    // Z
//    if (m_acceleration.z > m_maxAcceleration) {
//        m_correction = m_maxAcceleration / m_acceleration.z;
//        m_acceleration.z = m_maxAcceleration;
//        m_acceleration.x *= m_correction;
//    }
//    if (m_acceleration.z < -m_maxAcceleration) {
//        m_correction = -m_maxAcceleration / m_acceleration.z;
//        m_acceleration.z = -m_maxAcceleration;
//        m_acceleration.x *= m_correction;
//    }
//}

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

    // https://stackoverflow.com/questions/43960217/framerate-independent-acceleration-decceleration
    // physicsFrameTime independent aeroDrag + friction?
    {
        //// Apply aeroDrag
        //m_aeroDrag = 5.0f; // ?
        //if (m_isAirborne) {
        //    m_acceleration.x -= m_aeroDrag * m_speed.x * Engine::physicsFrameTime;
        //    m_acceleration.y -= m_aeroDrag * m_speed.y * Engine::physicsFrameTime;
        //    m_acceleration.z -= m_aeroDrag * m_speed.z * Engine::physicsFrameTime;
        //}

        //// Apply friction
        //m_dryFriction = 5.0f;
        //if (!m_isAirborne) {
        //    m_acceleration.x -= m_dryFriction * m_speed.x * Engine::physicsFrameTime;
        //    m_acceleration.y -= m_dryFriction * m_speed.y * Engine::physicsFrameTime;
        //    m_acceleration.z -= m_dryFriction * m_speed.z * Engine::physicsFrameTime;
        //}
    }

    m_speedLastFrame = m_speed;
    m_speed.x += m_acceleration.x;
    if (m_isAirborne)
        m_speed.y += (G::gravity + m_acceleration.y) * m_gravityBoost;
    else
        m_speed.y += (G::gravity + m_acceleration.y);
    m_speed.z += m_acceleration.z;

    // Apply aeroDrag
    if (m_isAirborne) {
        m_speed.x *= m_aeroDrag;
        m_speed.y *= m_aeroDrag;
        m_speed.z *= m_aeroDrag;
    }

    // Apply friction
    if (!m_isAirborne) {
        m_speed.x *= m_dryFriction;
        m_speed.y *= m_dryFriction;
        m_speed.z *= m_dryFriction;
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
    // Y - first because of potentially early returns below
    if (m_speed.y > m_maxJumpSpeed) {
        m_speed.y = m_maxJumpSpeed;
    }
    else if (m_speed.y < -m_maxJumpSpeed) {
        m_speed.y = -m_maxJumpSpeed;
    }
    if (m_speed.y != 0.0f && m_speed.y > -0.01f && m_speed.y < 0.01f)
        m_speed.y = 0.0f;

    // X & Z - Both m_forwardSpeed & m_rightSpeed hits m_maxCurrentSpeed simultaneously
    calculateForwardSpeed();
    calculateRightSpeed();
    if (m_forwardSpeed > m_maxCurrentSpeed && m_rightSpeed > m_maxCurrentSpeed) {
        m_speed.x = m_maxCurrentSpeed;
        m_speed.z = m_maxCurrentSpeed;
        return;
    }
    else if (m_forwardSpeed < -m_maxCurrentSpeed && m_rightSpeed < -m_maxCurrentSpeed) {
        m_speed.x = -m_maxCurrentSpeed;
        m_speed.z = -m_maxCurrentSpeed;
        return;
    }
    else if (m_forwardSpeed > m_maxCurrentSpeed && m_rightSpeed < -m_maxCurrentSpeed) {
        // TODO not sure what to do... only happens in extreme cases (huge accelerations etc.) does this happen IRL? 14-11-204
        //m_acceleration.x = G::camera->getFront().x * m_maxCurrentSpeed;
        //m_acceleration.z = G::camera->getFront().z * m_maxCurrentSpeed;
        m_speed = m_direction * m_maxCurrentSpeed; // does this make sense?
        return;
    }
    else if (m_forwardSpeed < -m_maxCurrentSpeed && m_rightSpeed > m_maxCurrentSpeed) {
        // TODO not sure what to do... only happens in extreme cases (huge accelerations etc.) does this happen IRL? 14-11-204
        m_speed = m_direction * m_maxCurrentSpeed; // does this make sense?
        return;
    }

    // X & Z
    float correction;
    if (m_forwardSpeed > m_maxCurrentSpeed) {
        correction = m_maxCurrentSpeed / m_forwardSpeed;
        m_speed.x *= correction;
        m_speed.z *= correction;
    }
    else if (m_forwardSpeed < -m_maxCurrentSpeed) {
        correction = -m_maxCurrentSpeed / m_forwardSpeed;
        m_speed.x *= correction;
        m_speed.z *= correction;
    }
    
    if (m_rightSpeed > m_maxStrafeCurrentSpeed) {
        correction = m_maxStrafeCurrentSpeed / m_rightSpeed;
        m_speed.x *= correction;
        m_speed.z *= correction;
    }
    else if (m_rightSpeed < -m_maxStrafeCurrentSpeed) {
        correction = -m_maxStrafeCurrentSpeed / m_rightSpeed;
        m_speed.x *= correction;
        m_speed.z *= correction;
    }

    // X & Z - snap low values to zero
    if (m_speed.x > -0.01f && m_speed.x < 0.01f)
        m_speed.x = 0.0f;
    if (m_speed.y > -0.01f && m_speed.y < 0.01f)
        m_speed.y = 0.0f;
    if (m_speed.z > -0.01f && m_speed.z < 0.01f)
        m_speed.z = 0.0f;
}

void Player::handleJump()
{  
    // TODO temp jump stuff
    
    // reset y acceleration once (not every physics tick) // move to resetAcceleration after implementing proper jumping?
    if (m_isAirborne)
        m_acceleration.y = 0.0f;
    else if (!m_isAirborne)
        m_acceleration.y = -G::gravity;
    
    static bool jumpStarted = false;
    if (m_isAirborne && G::camera->getPosition().y > 1.51f) {
        jumpStarted = true;
    }

    // landed on Floor // TODO add real collision check
    if (jumpStarted && m_isAirborne && G::camera->getPosition().y <= 1.5f || G::camera->getPosition().y <= -200.0f) {
        G::camera->setPositionY(1.5f);
        m_speed.y = 0.0f;
        m_speedLastFrame.y = 0.0f; // force the average speed to get to be exactly 0.0f, otherwise player will have residual speed and e.g.sink a little in the floor after landing
        m_acceleration.y = -G::gravity; // needed for just this 1 frame
        Engine::extrapolationResultPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        m_isAirborne = false;
        jumpStarted = false;
    }
}

void Player::updatePosition()
{
    //G::camera->setPosition(G::camera->getPosition() + ((G::player->getSpeed() + G::player->getSpeedLastFrame()) * 0.5f) * Engine::physicsFrameTime);

    // Collision test
    glm::vec3 proposedPosition = G::camera->getPosition() + ((G::player->getSpeed() + G::player->getSpeedLastFrame()) * 0.5f) * Engine::physicsFrameTime;

    AABB wall{
        glm::vec3( 10.0f,  10.0f, -2.5f), // m_vecMax
        glm::vec3(-10.0f, -10.0f, -3.5f)  // m_vecMin
    };

    if (Engine::AABBtoAABB(wall, G::player->getTAABB(proposedPosition)) == 1) {
        G::collisionTime = glfwGetTime();
        G::player->setSpeed(glm::vec3(0.0f, 0.0f, 0.0f));
    }
    else
        G::camera->setPosition(proposedPosition);
}

void Player::resetAcceleration()
{
    m_acceleration.x = 0.0f;
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

AABB Player::getTAABB(glm::vec3& position) // only to be used with player position because of offsets
{
    AABB x{};

    x.m_vecMax.x = position.x + 0.2f; // right
    x.m_vecMax.y = position.y + 0.2f; // top
    x.m_vecMax.z = position.z + 0.2f; // back

    x.m_vecMin.x = position.x - 0.2f; // left
    x.m_vecMin.y = position.y - 1.8f; // bottom
    x.m_vecMin.z = position.z - 0.2f; // front

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

void Player::calculateDirection()
{
    // retain last value when no movement
    if (G::camera->getPosition() == m_positionLastFrame)
        return;

    m_direction = glm::normalize(G::camera->getPosition() - m_positionLastFrame);
    m_positionLastFrame = G::camera->getPosition();
}