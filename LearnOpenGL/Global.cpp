#pragma once

#include "Global.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Global {

    GLenum glCheckError_(const char* file, int line)
    {
        GLenum errorCode{};
        while ((errorCode = glGetError()) != GL_NO_ERROR)
        {
            std::string error{};
            switch (errorCode)
            {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM";                  break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE";                 break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION";             break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW";                break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW";               break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY";                 break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            }
            std::println("ERROR: {}  | {} ({})", error, file, line);
        }
        return errorCode;
    }

    void glClearError()
    {
        while (glGetError() != GL_NO_ERROR);
    }

    int init(GLFWwindow* window)
    {
        assert(sizeof(unsigned int) == sizeof(GLuint) && "size of unsigned int and GLuint is not equal");

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorEnterCallback(window, cursor_enter_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetKeyCallback(window, key_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::println("Failed to initialize GLAD");
            return -1;
        }

        glEnable(GL_DEPTH_TEST);

        int flags{};
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }

        Global::glCheckError();

        return 0;
    }

    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            Global::camera.ProcessKeyboard(CameraMovement::FORWARD, Global::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            Global::camera.ProcessKeyboard(CameraMovement::BACKWARD, Global::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            Global::camera.ProcessKeyboard(CameraMovement::LEFT, Global::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            Global::camera.ProcessKeyboard(CameraMovement::RIGHT, Global::deltaTime);

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            Global::camera.ProcessKeyboard(CameraMovement::UP, Global::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            Global::camera.ProcessKeyboard(CameraMovement::DOWN, Global::deltaTime);
    }

#pragma warning( suppress : 4100 )
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_M && action == GLFW_PRESS) {
            if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                Global::windowsHasMouseFocus = false;
            }
            else if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }

        if (key == GLFW_KEY_V && action == GLFW_PRESS)
            glfwSwapInterval(0);

        if (key == GLFW_KEY_P && action == GLFW_PRESS)
            Global::paused = !Global::paused;

        static bool flashLightOn{ true }; // opposite of value set in main.cpp
        if (key == GLFW_KEY_F && action == GLFW_PRESS) {
            flashLight->setBool("flashLight.on", flashLightOn);
            flashLightOn = !flashLightOn;
        }

    }

#pragma warning( suppress : 4100 )
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        Global::windowWidth = width;
        Global::windowHeight = height;
        Global::camera.setAspectRatio((static_cast<float>(Global::windowWidth) / static_cast<float>(Global::windowHeight)));
        Global::camera.recalculateProjectionMatrix();
        Global::glCheckError();
    }

#pragma warning( suppress : 4100 )
    void cursor_enter_callback(GLFWwindow* window, int entered)
    {
        if (entered) {
            // The cursor entered the client area of the window
        }
        else {
            // The cursor left the client area of the window
            Global::windowsHasMouseFocus = false;
        }
        Global::glCheckError();
    }

#pragma warning( suppress : 4100 )
    void mouse_callback(GLFWwindow* window, double currentXPosIn, double currentYPosIn)
    {
        float currentXPos{ static_cast<float>(currentXPosIn) };
        float currentYPos{ static_cast<float>(currentYPosIn) };
        // last x,y position, initialize with center x,y coordinates
        static float lastXPos{ Global::windowWidth / 2.0f };
        static float lastYPos{ Global::windowHeight / 2.0f };

        if (!Global::windowsHasMouseFocus) {
            lastXPos = currentXPos;
            lastYPos = currentYPos;
            Global::windowsHasMouseFocus = true;
        }

        // Calculate the mouse's offset since the last frame
        float xoffset{ currentXPos - lastXPos };
        float yoffset{ lastYPos - currentYPos }; // reversed since y-coordinates go from bottom to top

        lastXPos = currentXPos;
        lastYPos = currentYPos;

        Global::camera.ProcessMouseMovement(xoffset, yoffset);
    }

#pragma warning( suppress : 4100 )
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Global::camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }

#pragma warning( suppress : 4100 )
    void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
    {
        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204)                   return;

        std::println("---------------");
        std::println("Debug message ({})", id);

        switch (source)
        {
        case GL_DEBUG_SOURCE_API:             std::println("Source: API");                  break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::println("Source: Window System");        break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::println("Source: Shader Compiler");      break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::println("Source: Third Party");          break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::println("Source: Application");          break;
        case GL_DEBUG_SOURCE_OTHER:           std::println("Source: Other");                break;
        }

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:               std::println("Type: Error");                break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::println("Type: Deprecated Behaviour"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::println("Type: Undefined Behaviour");  break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::println("Type: Portability");          break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::println("Type: Performance");          break;
        case GL_DEBUG_TYPE_MARKER:              std::println("Type: Marker");               break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::println("Type: Push Group");           break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::println("Type: Pop Group");            break;
        case GL_DEBUG_TYPE_OTHER:               std::println("Type: Other");                break;
        }

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:         std::println("Severity: high");                break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::println("Severity: medium");              break;
        case GL_DEBUG_SEVERITY_LOW:          std::println("Severity: low");                 break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::println("Severity: notification");        break;
        }
    }
}