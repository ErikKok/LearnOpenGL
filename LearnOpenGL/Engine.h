#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine { // Class? // TODO use doubles?
    inline int ticksLoop{ 0 };
    inline int ticksPhysics{ 0 };
    inline float currentFrameTime{ static_cast<float>(glfwGetTime()) };
    inline float totalTimePassed = 0.0f;
    const inline float physicsFrameTime = 0.01f; // in seconds
    const inline float physicsFrameTimeFactor = 1.0f;
    inline float frameTimeRemaining = 0.0f; // {}
    inline float interpolationFactor{ 1.0f };  // TODO double?
    inline float interpolationResultPositionY{ 1.0f };
    inline bool useInterpolationResultPositionY{ true }; // flashlight gets funky otherwise

    void perFrameTimeLogic();

    void processInput(GLFWwindow* window);

    // Following functions are called by glfwPollEvents()
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void cursor_enter_callback(GLFWwindow* window, int entered);
    void mouse_callback(GLFWwindow* window, double currentXPosIn, double currentYPosIn);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
}