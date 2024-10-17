#pragma once

#include "Engine.h"
#include "Global.h"
#include "GlobalEntities.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

void Engine::perFrameTimeLogic()
{
    currentFrameTime = static_cast<float>(glfwGetTime());
    G::deltaTime = currentFrameTime - G::timestampLastFrame;
    if (G::deltaTime > 0.1f) { // smooths huge spikes out
        std::println("WARNING: deltaTime too high! Capped at 10ms, original value was: {}ms", G::deltaTime);
        G::deltaTime = 0.1f;
    }
    G::timestampLastFrame = currentFrameTime;
    frameTimeRemaining += G::deltaTime;
}

void Engine::doPhysics()
{   
    //if (frameTimeRemaining < physicsFrameTime) {
    //    GE::player.resetAcceleration();
    //    return;
    //}

    while (frameTimeRemaining >= physicsFrameTime)
    {
        ticksPhysics++;
        GE::player.limitAcceleration();
        GE::player.handleMovement(); // -> limitSpeed(); -> resetAcceleration();
        totalTimePassed += physicsFrameTime;
        frameTimeRemaining -= physicsFrameTime;
    }
    extrapolationFactor = frameTimeRemaining / physicsFrameTime;
    std::println("extrapolationFactor: {}", extrapolationFactor);
}

void Engine::doExtrapolationStep()
{
    isExtrapolationStep = true;
    extrapolationResultPosition = GE::player.getSpeed() * extrapolationFactor * physicsFrameTime;
    std::println("extrapolationResultPositionY: {}", extrapolationResultPosition.y);
}

glm::vec3 Engine::follow(const glm::vec3& origin, const glm::vec3& destination)
{
    //glm::vec3 newPosition = origin + (3.0f * G::deltaTime) * glm::normalize(destination - origin);
    //std::println("newPosition: {}, {}, {}", newPosition.x, newPosition.y, newPosition.z);

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

    //Check if Box1's max is greater than Box2's min and Box1's min is less than Box2's max
    return(box1.m_vecMax.x > box2.m_vecMin.x &&
           box1.m_vecMin.x < box2.m_vecMax.x &&
           box1.m_vecMax.y > box2.m_vecMin.y &&
           box1.m_vecMin.y < box2.m_vecMax.y &&
           box1.m_vecMax.z > box2.m_vecMin.z &&
           box1.m_vecMin.z < box2.m_vecMax.z);
}

void Engine::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        GE::player.initMovement(CameraMovement::FORWARDBACKWARD);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        GE::player.initMovement(CameraMovement::FORWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        GE::player.initMovement(CameraMovement::BACKWARD);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        GE::player.initMovement(CameraMovement::LEFTRIGHT);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        GE::player.initMovement(CameraMovement::LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        GE::player.initMovement(CameraMovement::RIGHT);

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        GE::player.initMovement(CameraMovement::UPDOWN);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        GE::player.initMovement(CameraMovement::UP);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        GE::player.initMovement(CameraMovement::DOWN);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        GE::player.initMovement(CameraMovement::JUMP);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        GE::player.setMaxCurrentSpeed(5.0f);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        GE::player.setMaxCurrentSpeed(GE::player.getMaxCurrentSpeed());

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
};

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
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            G::windowsHasMouseFocus = false;
        }
        else if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    GE::camera.setAspectRatio((static_cast<float>(G::windowWidth) / static_cast<float>(G::windowHeight)));
    G::glCheckError();
}

#pragma warning( suppress : 4100 )
void Engine::cursor_enter_callback(GLFWwindow* window, int entered)
{
    if (entered) {
        // The cursor entered the client area of the window
    }
    else {
        // The cursor left the client area of the window
        G::windowsHasMouseFocus = false;
    }
    G::glCheckError();
}

#pragma warning( suppress : 4100 )
void Engine::mouse_callback(GLFWwindow* window, double currentXPosIn, double currentYPosIn)
{
    float currentXPos{ static_cast<float>(currentXPosIn) };
    float currentYPos{ static_cast<float>(currentYPosIn) };
    // last x,y position, initialize with center x,y coordinates
    static float lastXPos{ G::windowWidth / 2.0f };
    static float lastYPos{ G::windowHeight / 2.0f };

    if (!G::windowsHasMouseFocus) {
        lastXPos = currentXPos;
        lastYPos = currentYPos;
        G::windowsHasMouseFocus = true;
    }

    // Calculate the mouse's offset since the last frame
    float xoffset{ currentXPos - lastXPos };
    float yoffset{ lastYPos - currentYPos }; // reversed since y-coordinates go from bottom to top

    lastXPos = currentXPos;
    lastYPos = currentYPos;

    GE::camera.processMouseMovement(xoffset, yoffset);
}

#pragma warning( suppress : 4100 )
void Engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    GE::camera.processMouseScroll(static_cast<float>(yoffset));
}