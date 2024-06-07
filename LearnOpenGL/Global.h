#pragma once

#include "Camera.h"
#include "Light.h"

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

struct testSSBO {
    int x;
    float y;
    glm::vec4 color;
    int xxx;
    int yyy;
};

namespace Global {
    // Window
    inline int windowWidth{ 1920 };
    inline int windowHeight{ 1080 };
    inline bool windowsHasMouseFocus{ false };

    // Camera
    inline glm::vec3 cameraPosition{ 0.0f, 1.5f, 15.0f }; // TODO init here for now
    inline glm::vec3 flashLightShadowOffset{ 0.4f, -0.5f, -0.3f }; // TODO flashLight shines from the player camera (0,0,0), but casts shadow from this camera with this offset
    inline Camera camera((static_cast<float>(windowWidth) / static_cast<float>(windowHeight)), cameraPosition); // 8.0f, 1.5f, 8.0f
    // in global because of toggle
    inline Camera cameraFlashLight(1.77f, cameraPosition + flashLightShadowOffset); // TODO get aspectratio from depthmap texture

    // Renderloop
    inline GLfloat deltaTime{ 0.0f };	// time between current frame and last frame
    inline GLfloat lastFrame{ 0.0f };
    inline bool paused{ false };

    // Shader
    inline GLuint shaderCurrentlyActive{};

    // Toggles
    inline bool drawOutline{ false };
    inline bool debugQuadVisible{ false };
    inline bool flashLightOnUpdated{ true };
    inline bool frustumVisible{ false };

    // Error
    const GLenum glCheckError_(const char* file, int line);
    #define glCheckError() glCheckError_(__FILE__, __LINE__)
    void glClearError();

    // Matrices
    const glm::mat4 getModelMatrix(glm::vec3 translate, float rotateDegrees, glm::vec3 rotateVec3, glm::vec3 scale);
    const glm::mat4 getModelViewMatrix(glm::vec3 translate, float rotateDegrees, glm::vec3 rotateVec3, glm::vec3 scale);

    // Init
    void initStencilBuffer();
    void clearStencilBuffer();

    // https://www.learncpp.com/cpp-tutorial/pass-by-const-lvalue-reference/ - The cost of pass by value vs pass by reference
    void cheap2Copy();

    const int init(GLFWwindow* window);
    void processInput(GLFWwindow* window);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void cursor_enter_callback(GLFWwindow* window, int entered);
    void mouse_callback(GLFWwindow* window, double currentXPosIn, double currentYPosIn);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
    void getInformation();
}