#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory> // for std::unique_ptr and std::make_unique

// Shared and Unique pointers
// Used for game engine specific references/assets only, which maybe will be handled by an asset manager in the future
// See https://youtu.be/HkGZ378nArE?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

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

class Camera;
class Player;

namespace G {
    // Entities
    inline Camera* camera{ nullptr };
    inline Player* player{ nullptr };

    // Window
    inline int windowWidth{ 1920 };
    inline int windowHeight{ 1080 };
    inline bool isMouseCursorEnabled{ true };
    inline bool firstMouse{ true };
    inline float lastXPos{ 0.0f };
    inline float lastYPos{ 0.0f };

    // Misc
    inline float outlineAlpha{ 0.0f };

    // Camera
    inline glm::vec3 cameraInitialPosition{ 0.0f, 1.5f, 15.0f };

    // SSBO - cache for various matrix calculations
    inline glm::mat4 modelViewMatrixTemp{};

    // Renderloop
    inline GLfloat deltaTime{ 0.0f };               // Time in seconds between current frame and last frame
    inline GLfloat timestampLastFrame{ 0.0f };      // Timestamp of last frame
    const inline float gravity{ -9.81f };           // 9.80665
    inline bool paused{ false };

    // Shader
    inline GLuint shaderCurrentlyActive{};

    // Toggles
    inline bool drawOutline{ false };
    inline int frustumVisible{ 0 };
    inline int debugQuadVisible{ 0 };
    inline bool isFlashLightOnUpdated{ true };
    inline bool isVSyncEnabled{ false };

    // Error | Debug
    const GLenum glCheckError_(const char* file, int line);
    #define glCheckError() glCheckError_(__FILE__, __LINE__)
    void glClearError();
    void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

    // Matrices
    glm::mat4 calculateModelMatrix(glm::vec3 translate, float rotateDegrees, glm::vec3 rotateVec3, glm::vec3 scale);
    glm::mat4 calculateModelViewMatrix(glm::vec3 translate, float rotateDegrees, glm::vec3 rotateVec3, glm::vec3 scale);
    void applyCameraOffset(Camera* camera, float x, float y, float z);

    void cheap2Copy(); // https://www.learncpp.com/cpp-tutorial/pass-by-const-lvalue-reference/ - The cost of pass by value vs pass by reference
    void getInformation();

    // ImGui

    // https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html
    // https://www.jakobmaier.at/posts/imgui-slider-with-getter-and-setters/

    // https://alvinalexander.com/programming/printf-format-cheat-sheet/
    // %d int
    // %g insignificant zeroes to the right of the decimal point are not included
    // %0+7.2f = seven-wide (incl. .), two positions after the decimal, always signed

    void ImGui();
    //inline bool test{ false };
    inline double collisionTime{};
}