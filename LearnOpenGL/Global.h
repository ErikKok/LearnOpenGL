#pragma once

#include "Camera.h"
#include "Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <print>

// TODO - in de namespace plaatsen?
template <typename T>
using SPtr = std::shared_ptr<T>;

template <typename T>
using UPtr = std::unique_ptr<T>;

// TODO - niet meer global
class Shader;
inline Shader* flashLight{ nullptr }; // points to multiLight

namespace Global {
    inline int windowWidth{ 1920 };
    inline int windowHeight{ 1080 };
    inline Camera camera((static_cast<float>(windowWidth) / static_cast<float>(windowHeight)), glm::vec3(8.0f, 1.5f, 8.0f));
    inline bool windowsHasMouseFocus{ false };

    inline GLfloat deltaTime{ 0.0f };	// time between current frame and last frame
    inline GLfloat lastFrame{ 0.0f };
    inline bool paused{ false };
    inline bool drawOutline{ false };

    GLenum glCheckError_(const char* file, int line);
    #define glCheckError() glCheckError_(__FILE__, __LINE__)
    void glClearError();

    void transform(const Shader& shader, glm::vec3 translate, float rotateDegrees, glm::vec3 rotateVec3, glm::vec3 scale, const glm::mat4& view);
    void transformNormalViewCPU(const Shader& shader, glm::vec3 translate, float rotateDegrees, glm::vec3 rotateVec3, glm::vec3 scale, const glm::mat4& view);
    void initStencilBuffer();
    void clearStencilBuffer();


    int init(GLFWwindow* window);
    void processInput(GLFWwindow* window);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void cursor_enter_callback(GLFWwindow* window, int entered);
    void mouse_callback(GLFWwindow* window, double currentXPosIn, double currentYPosIn);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
    void getBound();
}