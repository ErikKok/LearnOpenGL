#pragma once

#include "Camera.h"
#include "Light.h"
#include "Player.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <print>

// Shared and Unique pointers
// Used for game engine specific references/assets only, which maybe will be handled by an asset manager in the future
// See https://youtu.be/HkGZ378nArE?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT
// TODO - in de namespace plaatsen?

// Usage
// Class data member: std::unique_ptr<Type> name{ nullptr };
// Initialize: name = std::make_unique<Type>(optional arguments);
// 'Stand alone':
// std::unique_ptr name{ std::make_unique<Type>(optional arguments) };
// auto name{ std::make_unique<Type>(optional arguments) }; (shorter, same result)
//
//.get() = get a raw resource pointer from a std::unique_ptr

template <typename T>
using SPtr = std::shared_ptr<T>;

template <typename T>
using UPtr = std::unique_ptr<T>;

////////////////////////

namespace G {
    // Window
    inline int windowWidth{ 1920 };
    inline int windowHeight{ 1080 };
    inline bool windowsHasMouseFocus{ false };

    // Misc
    inline float outlineAlpha{ 0.0f };
    inline Player player{};

    // Camera
    inline glm::vec3 cameraInitialPosition{ 0.0f, 1.5f, 15.0f };
    inline Camera camera((static_cast<float>(windowWidth) / static_cast<float>(windowHeight)), cameraInitialPosition);

    // SSBO - cache for various matrix calculations
    inline glm::mat4 modelViewMatrixTemp{};

    // Renderloop
    inline GLfloat deltaTime{ 0.0f };               // Time in seconds between current frame and last frame
    inline GLfloat deltaTimeLastFrame{ 0.0f };      // Timestamp of last frame

    inline float gravity{ -9.81f };                 // 9.80665
    inline bool paused{ false };

    // Shader
    inline GLuint shaderCurrentlyActive{};

    // Toggles
    inline bool drawOutline{ false };
    inline int frustumVisible{ 0 };
    inline int debugQuadVisible{ 0 };
    inline bool isFlashLightOnUpdated{ true };

    // Error
    const GLenum glCheckError_(const char* file, int line);
    #define glCheckError() glCheckError_(__FILE__, __LINE__)
    void glClearError();

    // Matrices
    glm::mat4 calculateModelMatrix(glm::vec3 translate, float rotateDegrees, glm::vec3 rotateVec3, glm::vec3 scale);
    glm::mat4 calculateModelViewMatrix(glm::vec3 translate, float rotateDegrees, glm::vec3 rotateVec3, glm::vec3 scale);
    void applyCameraOffset(Camera* camera, float x, float y, float z);

    // Init
    // https://www.learncpp.com/cpp-tutorial/pass-by-const-lvalue-reference/ - The cost of pass by value vs pass by reference
    void cheap2Copy();

    int init(GLFWwindow* window);
    void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
    void getInformation();

    void processInput(GLFWwindow* window);
    // Following functions are called by glfwPollEvents()
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void cursor_enter_callback(GLFWwindow* window, int entered);
    void mouse_callback(GLFWwindow* window, double currentXPosIn, double currentYPosIn);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
}