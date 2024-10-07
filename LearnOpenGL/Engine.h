#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {
    void processInput(GLFWwindow* window);

    // Following functions are called by glfwPollEvents()
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void cursor_enter_callback(GLFWwindow* window, int entered);
    void mouse_callback(GLFWwindow* window, double currentXPosIn, double currentYPosIn);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
}