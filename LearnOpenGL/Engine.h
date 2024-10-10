#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace Engine { // Class? // TODO use doubles?
    inline int ticksLoop{ 0 };
    inline int ticksPhysics{ 0 };
    inline float currentFrameTime{ static_cast<float>(glfwGetTime()) }; // in seconds since runtime
    inline float totalTimePassed{ 0.0f }; // in seconds
    const inline float physicsFrameTime{ 0.01f }; // in seconds
    inline float frameTimeRemaining{ 0.0f }; // in seconds

    // https://gamedev.stackexchange.com/a/187668
    inline float extrapolationFactor{ 1.0f };
    inline glm::vec3 extrapolationResultPosition{ 0.0f, 0.0f, 0.0f }; // for Player and flashlight

    void perFrameTimeLogic();
    void doPhysics();
    void processInput(GLFWwindow* window);

    // Following functions are called by glfwPollEvents()
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void cursor_enter_callback(GLFWwindow* window, int entered);
    void mouse_callback(GLFWwindow* window, double currentXPosIn, double currentYPosIn);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
}