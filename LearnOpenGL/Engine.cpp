#pragma once
#include "Engine.h"

#include "Camera.h"
#include "Global.h"
#include "Player.h"

#include <print>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void Engine::perFrameTimeLogic()
{
    currentFrameTime = static_cast<float>(glfwGetTime());
    G::deltaTime = currentFrameTime - G::timestampLastFrame;
    if (G::deltaTime > 0.1f) { // smooths huge spikes out
        std::println("WARNING: deltaTime too high! Capped at 10ms, original value was: {}ms", G::deltaTime * 1000);
        G::deltaTime = 0.1f;
    }
    G::timestampLastFrame = currentFrameTime;
    frameTimeRemaining += G::deltaTime;
}

void Engine::processInput(GLFWwindow* window)
{
    // FORWARD BACKWARD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        ; // G::player->initMovement(PlayerMovement::forwardbackward); (do nothing, no acceleration applied)
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        G::player->initMovement(PlayerMovement::forward);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        G::player->initMovement(PlayerMovement::backward);
    
    // LEFT RIGHT
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        ; // G::player->initMovement(PlayerMovement::leftright); (do nothing, no acceleration applied)
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        G::player->initMovement(PlayerMovement::left);
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        G::player->initMovement(PlayerMovement::right);
    
    // UP DOWN
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        G::player->initMovement(PlayerMovement::updown);
    else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        G::player->initMovement(PlayerMovement::up);
    else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        G::player->initMovement(PlayerMovement::down);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        G::player->initMovement(PlayerMovement::jump);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        G::player->setMaxCurrentSpeed(G::player->getWalkSpeed());
        G::player->setMaxStrafeCurrentSpeed(G::player->getStrafeWalkSpeed());
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        G::player->setMaxCurrentSpeed(G::player->getRunSpeed());
        G::player->setMaxStrafeCurrentSpeed(G::player->getStrafeRunSpeed());
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
};

void Engine::doPhysics()
{   
    while (frameTimeRemaining >= physicsFrameTime)
    {
        ticksPhysics++;
        G::player->calculateSpeed();
        G::player->limitSpeed();
        G::player->resetAcceleration();
        G::player->handleJump();
        G::player->updatePosition();

        totalTimePassed += physicsFrameTime;
        frameTimeRemaining -= physicsFrameTime;
    }

    G::player->resetAcceleration();
    extrapolationFactor = frameTimeRemaining / physicsFrameTime;
}

void Engine::doExtrapolationStep()
{
    isExtrapolationStep = true;
    extrapolationResultPosition = G::player->getSpeed() * extrapolationFactor * physicsFrameTime;
}

glm::vec3 Engine::follow(const glm::vec3& origin, const glm::vec3& destination)
{
    return origin + (3.0f * G::deltaTime) * glm::normalize(destination - origin);
}

bool Engine::isEqual(const glm::vec3& position1, const glm::vec3& position2, float epsilon)
{
    return  std::abs(position1.x - position2.x) < epsilon &&
            std::abs(position1.y - position2.y) < epsilon &&
            std::abs(position1.z - position2.z) < epsilon;
}

bool Engine::AABBtoAABB(const AABB& box1, const AABB& box2)
{
    // Check if box1's max is greater than box2's min and box1's min is less than box2's max
    // true if collision
    return(box1.m_vecMax.x > box2.m_vecMin.x &&
           box1.m_vecMin.x < box2.m_vecMax.x &&
           box1.m_vecMax.y > box2.m_vecMin.y &&
           box1.m_vecMin.y < box2.m_vecMax.y &&
           box1.m_vecMax.z > box2.m_vecMin.z &&
           box1.m_vecMin.z < box2.m_vecMax.z);
}

int Engine::AABBtoAABBAxis(const AABB& box1, const AABB& box2)
{
    if(box1.m_vecMax.x > box2.m_vecMin.x &&
        box1.m_vecMin.x < box2.m_vecMax.x &&
        box1.m_vecMax.y > box2.m_vecMin.y &&
        box1.m_vecMin.y < box2.m_vecMax.y &&
        box1.m_vecMax.z > box2.m_vecMin.z &&
        box1.m_vecMin.z < box2.m_vecMax.z == 0)
    return 0;
    
    if (box1.m_vecMax.x > box2.m_vecMin.x && box1.m_vecMin.x < box2.m_vecMax.x)
        return 1;
    if (box1.m_vecMax.y > box2.m_vecMin.y && box1.m_vecMin.y < box2.m_vecMax.y) 
        return 2;
    if (box1.m_vecMax.z > box2.m_vecMin.z && box1.m_vecMin.z < box2.m_vecMax.z)
        return 3;

    return 0;
}

#pragma warning( suppress : 4100 )
void Engine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        G::isFlashLightOnUpdated = false;
        return;
    }

    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        G::frustumVisible++;
        if (G::frustumVisible == 4)
            G::frustumVisible = 0;
        return;
    }

    static int polygonMode{ 0 };
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        if (polygonMode == 0) {
            std::println("Polygon Mode LINE");
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            polygonMode = 1;
            return;
        }
        if (polygonMode == 1) {
            std::println("Polygon Mode POINT");
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            polygonMode = 2;
            return;
        }
        if (polygonMode == 2) {
            std::println("Polygon Mode FILL");
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            polygonMode = 0;
            return;
        }
        return;
    }

    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        if (G::isMouseCursorEnabled) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            G::isMouseCursorEnabled = false;
            G::firstMouse = true;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            G::isMouseCursorEnabled = true;
        }
        return;
    }

    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        G::drawOutline = !G::drawOutline;
        return;
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        G::paused = !G::paused;
        return;
    }

    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        G::debugQuadVisible++;
        if (G::debugQuadVisible == 4)
            G::debugQuadVisible = 0;
        return;
    }

    if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        if (G::isVSyncEnabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        G::isVSyncEnabled = !G::isVSyncEnabled;
        return;
    }
}

#pragma warning( suppress : 4100 )
void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    G::windowWidth = width;
    G::windowHeight = height;
    G::camera->setAspectRatio((static_cast<float>(G::windowWidth) / static_cast<float>(G::windowHeight)));
    G::glCheckError();
}

#pragma warning( suppress : 4100 )
void Engine::cursor_enter_callback(GLFWwindow* window, int entered)
{
    if (entered) {
        // The cursor entered the client area of the window
        G::firstMouse = true;

        // Uncomment to automatically gain control (instead of pressing M first)
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        //G::isMouseCursorEnabled = true;
    }
    else {
        // The cursor left the client area of the window
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        G::isMouseCursorEnabled = false;
    }
}

#pragma warning( suppress : 4100 )
void Engine::mouse_callback(GLFWwindow* window, double currentXPosIn, double currentYPosIn)
{
    if (!G::isMouseCursorEnabled)
        return;
    
    // cast double to float
    float currentXPos{ static_cast<float>(currentXPosIn) };
    float currentYPos{ static_cast<float>(currentYPosIn) };

    // use the current mouse's coordinates to nullify the offsets for processMouseMovement() to prevent sudden movement
    if (G::firstMouse) {
        G::lastXPos = currentXPos;
        G::lastYPos = currentYPos;
        G::firstMouse = false;
    }

    // Calculate the mouse's offset since the last frame
    G::camera->processMouseMovement(currentXPos - G::lastXPos, G::lastYPos - currentYPos); // Y is reversed since y-coordinates go from bottom to top

    G::lastXPos = currentXPos;
    G::lastYPos = currentYPos;
}

#pragma warning( suppress : 4100 )
void Engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    G::camera->processMouseScroll(static_cast<float>(yoffset));
}