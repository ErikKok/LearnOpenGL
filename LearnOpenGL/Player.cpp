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
    //GE::camera.updateCameraVectors();
    float maxAcceleration{ 60.0f };
    if (direction == CameraMovement::FORWARD) {
        m_acceleration.x = GE::camera.getFront().x * maxAcceleration;
        m_acceleration.z = GE::camera.getFront().z * maxAcceleration;
        //m_position.x += m_front.x * velocity;
        //m_position.z += m_front.z * velocity;
    }
    if (direction == CameraMovement::BACKWARD) {
        m_acceleration.x = GE::camera.getFront().x * -maxAcceleration;
        m_acceleration.z = GE::camera.getFront().z * -maxAcceleration;
    }
    //if (direction == CameraMovement::LEFT) {
    //    m_position.x -= m_right.x * velocity;
    //    m_position.z -= m_right.z * velocity;
    //}
    //if (direction == CameraMovement::RIGHT) {
    //    m_position.x += m_right.x * velocity;
    //    m_position.z += m_right.z * velocity;
    //}

    /// if aan, dan geen afbuiging na bewegen muis
    /// if uit, dan zelfde snelheid met v sync uit

    //if (m_acceleration.x > 20.0f) // if running
    //    m_acceleration.x = 20.0f;
    //if (m_acceleration.z > 20.0f)
    //    m_acceleration.z = 20.0f;

    //if (m_acceleration.x < -20.0f)
    //    m_acceleration.x = -20.0f;
    //if (m_acceleration.z < -20.0f)
    //    m_acceleration.z = -20.0f;

    // correct both axis in same ratio if one succeeds the max
    // needs to be in init() instead of handle(), otherwise it's influenced by framerate


    float correctionX;
    if (m_acceleration.x > maxAcceleration) {// if running
        correctionX = m_acceleration.x / maxAcceleration;
        m_acceleration.x = maxAcceleration;
        m_acceleration.z = m_acceleration.z / correctionX;
    }
    float correctionZ;
    if (m_acceleration.z > maxAcceleration) {
        correctionZ = m_acceleration.z / maxAcceleration;
        m_acceleration.z = maxAcceleration;
        m_acceleration.x = m_acceleration.x / correctionZ;
    }

    if (m_acceleration.x < -maxAcceleration) {// if running
        correctionX = m_acceleration.x / -maxAcceleration;
        m_acceleration.x = -maxAcceleration;
        m_acceleration.z = m_acceleration.z / correctionX;
    }

    if (m_acceleration.z < -maxAcceleration) {
        correctionZ = m_acceleration.z / -maxAcceleration;
        m_acceleration.z = -maxAcceleration;
        m_acceleration.x = m_acceleration.x / correctionZ;
    }

    std::println("player.m_acceleration: {}, {}, {}", GE::player.m_acceleration.x, GE::player.m_acceleration.y, GE::player.m_acceleration.z);

}

void Player::initJump()
{
    if (m_isJumping == false) { // TODO + check if touching ground
        m_acceleration.y = 200.0f;
        m_isJumping = true;
    }
}

void Player::handleMovement()
{
    //std::println("player.m_acceleration: {}, {}, {}", GE::player.m_acceleration.x, GE::player.m_acceleration.y, GE::player.m_acceleration.z);
    //if (m_acceleration.x == 0.0f && m_acceleration.z == 0.0f)
    //    return;

    // todo na stilstand is initiele richting fout



    //if (m_acceleration.x < -20.0f)
    //    m_acceleration.x = -20.0f;
    //if (m_acceleration.z < -20.0f)
    //    m_acceleration.z = -20.0f;

    m_SpeedLastFrame = m_Speed;
    m_Speed.x += m_acceleration.x * Engine::physicsFrameTime;// *0.5f;
    m_Speed.z += m_acceleration.z * Engine::physicsFrameTime;// *0.5f;
    GE::camera.setPosition(GE::camera.getPosition() + (((m_Speed + m_SpeedLastFrame) / 2.0f) * Engine::physicsFrameTime));
    //m_Speed.x += m_acceleration.x * Engine::physicsFrameTime * 0.5f;
    //m_Speed.z += m_acceleration.z * Engine::physicsFrameTime * 0.5f;

    //if (m_acceleration.x > 20.0f) // if running
    //    m_acceleration.x = 20.0f;
    //if (m_acceleration.z > 20.0f)
    //    m_acceleration.z = 20.0f;

    //if (m_acceleration.x < -20.0f)
    //    m_acceleration.x = -20.0f;
    //if (m_acceleration.z < -20.0f)
    //    m_acceleration.z = -20.0f;

    //if (m_Speed.x > 10.0f)
    //    m_Speed.x = 10.0f;
    //if (m_Speed.y > 10.0f)
    //    m_Speed.y = 10.0f;
    //if (m_Speed.z > 10.0f)
    //    m_Speed.z = 10.0f;

    //if (m_Speed.x < -10.0f)
    //    m_Speed.x = -10.0f;
    //if (m_Speed.y < -10.0f)
    //    m_Speed.y = -10.0f;
    //if (m_Speed.z < -10.0f)
    //    m_Speed.z = -10.0f;


    if (m_acceleration.x != 0.0f)
        m_acceleration.x *= 10.0f * Engine::physicsFrameTime; // drag 
    if (m_acceleration.z != 0.0f)
        m_acceleration.z *= 10.0f * Engine::physicsFrameTime; // drag 

    //if (m_acceleration.x >= -0.000001f && m_acceleration.x <= 0.000001f) {
    //    m_acceleration.x = 0.0f;
    //}
    //if (m_acceleration.z >= -0.000001f && m_acceleration.z <= 0.000001f) {
    //    m_acceleration.z = 0.0f;
    //}

    m_Speed.x *= 95.0f * Engine::physicsFrameTime; // drag 
    m_Speed.z *= 95.0f * Engine::physicsFrameTime; // drag


    // zorgt voor afbuiging IF !vsync && muis bewogen, bij te lage waardes merkbaar (10.0f bijv)
    //if (m_Speed.x > 100.0f) // if running
    //    m_Speed.x = 100.0f;
    //if (m_Speed.z > 100.0f)
    //    m_Speed.z = 100.0f;

    //if (m_Speed.x < -100.0f)
    //    m_Speed.x = -100.0f;
    //if (m_Speed.z < -100.0f)
    //    m_Speed.z = -100.0f;

    float maxSpeed{ 40.0f };
    float correctionX2;
    //if (m_Speed.x > maxSpeed && (m_Speed.z > maxSpeed) ? TODO ook negatief
    if (m_Speed.x > maxSpeed) {// if running
        correctionX2 = m_Speed.x / maxSpeed;
        m_Speed.x = maxSpeed;
        m_Speed.z = m_Speed.z / correctionX2;
    }
    float correctionZ2;
    if (m_Speed.z > maxSpeed) {
        correctionZ2 = m_Speed.z / maxSpeed;
        m_Speed.z = maxSpeed;
        m_Speed.x = m_Speed.x / correctionZ2;
    }

    if (m_Speed.x < -maxSpeed) {// if running
        correctionX2 = m_Speed.x / -maxSpeed;
        m_Speed.x = -maxSpeed;
        m_Speed.z = m_Speed.z / correctionX2;
    }

    if (m_Speed.z < -maxSpeed) {
        correctionZ2 = m_Speed.z / -maxSpeed;
        m_Speed.z = -maxSpeed;
        m_Speed.x = m_Speed.x / correctionZ2;
    }

    std::println("player.m_Speed: {}, {}, {}", GE::player.m_Speed.x, GE::player.m_Speed.y, GE::player.m_Speed.z);

}

void Player::handleJump()
{
    if (m_isJumping == true) {
        m_SpeedLastFrame = m_Speed;
        m_Speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime * 0.5f;
        GE::camera.setPositionY(GE::camera.getPosition().y + ((m_Speed.y + m_SpeedLastFrame.y) / 2) * Engine::physicsFrameTime);
        m_Speed.y += (G::gravity + m_acceleration.y) * Engine::physicsFrameTime * 0.5f;

        if (m_isJumping == true && GE::camera.getPosition().y <= 1.5f) { // landed on Floor // TODO add real collision check
            m_acceleration.y = -G::gravity;
            GE::camera.setPositionY(1.5f);
            m_Speed.y = 0.0f;
            Engine::extrapolationResultPosition = glm::vec3(0.0f, 0.0f, 0.0f);
            m_isJumping = false;
        }

        if (G::gravity + m_acceleration.y != 0.0f)
            m_acceleration.y *= m_drag;
    }
    //std::println("yVelocity: {}", m_ySpeed);
    //std::println("Netto acceleration: {}", (G::gravity + m_acceleration));
}

void Player::handleJumpextrapolation()
{
    if (m_isJumping == true) {
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