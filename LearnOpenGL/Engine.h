#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

struct AABB
{
    glm::vec3 m_vecMax;
    glm::vec3 m_vecMin;
};

namespace Engine { // Class?
    inline int ticksLoop{ 0 };
    inline int ticksPhysics{ 0 };
    inline float currentFrameTime{ static_cast<float>(glfwGetTime()) }; // in seconds since runtime
    inline float totalTimePassed{ 0.0f }; // in seconds
    const inline float physicsFrameTime{ 0.01f }; // in seconds // TODO use doubles for physic / time related stuff?
    inline float frameTimeRemaining{ 0.0f }; // in seconds

    inline bool skip;

    // https://gamedev.stackexchange.com/a/187668
    inline bool isExtrapolationStep{ true };
    inline float extrapolationFactor{ 1.0f };
    inline glm::vec3 extrapolationResultPosition{ 0.0f, 0.0f, 0.0f }; // for Player and flashlight

    void perFrameTimeLogic();
    void doPhysics();
    void doExtrapolationStep();
    glm::vec3 follow(const glm::vec3& origin, const glm::vec3& destination);
    bool isEqual(const glm::vec3& p1, const glm::vec3& p2, float epsilon = 0.01f);
    bool AABBtoAABB(const AABB& tBox1, const AABB& tBox2);
    void processInput(GLFWwindow* window);

    // Following functions are called by glfwPollEvents()
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void cursor_enter_callback(GLFWwindow* window, int entered);
    void mouse_callback(GLFWwindow* window, double currentXPosIn, double currentYPosIn);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
}