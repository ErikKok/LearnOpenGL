#pragma once

#include "Global.h"
#include "Camera.h"

GLenum Global::glCheckError_(const char* file, int line)
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

void Global::glClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

// Takes in full transformation parameters in World space, and outputs model in World space
glm::mat4 Global::getModelMatrix(glm::vec3 translate, float rotateDegrees, glm::vec3 rotateVec3, glm::vec3 scale)
{
    glm::mat4 model{ 1.0f };
    if (translate != glm::vec3(0.0f, 0.0f, 0.0f))
        model = glm::translate(model, translate);
    if (rotateVec3 != glm::vec3(0.0f, 0.0f, 0.0f))
        model = glm::rotate(model, glm::radians(rotateDegrees), rotateVec3);
    if (scale != glm::vec3(0.0f, 0.0f, 0.0f))
        model = glm::scale(model, scale);
    return model;
}

// Takes in full transformation parameters in World space, and outputs model in View space
glm::mat4 Global::getModelViewMatrix(glm::vec3 translate, float rotateDegrees, glm::vec3 rotateVec3, glm::vec3 scale)
{ 
    return Global::camera.getViewMatrix() * getModelMatrix(translate, rotateDegrees, rotateVec3, scale);
}

void Global::initStencilBuffer() {
    glEnable(GL_STENCIL_TEST);
    // if both stencil test and depth test succeed replace stencil value
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    // all fragments should pass the stencil test until needed
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    // some information:
    // to actually change the depth clearing value, you need to call glClearDepth() before calling glClear()
    // 
    // https://a.disquscdn.com/uploads/mediaembed/images/2106/4734/original.jpg
    // glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
    // glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_REPLACE);
}

void Global::clearStencilBuffer() {
    // enable writing to the stencil buffer, so it can be cleared
    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT); // TODO combine with other glClear commands, probably faster?
    // disable writing to the stencil buffer until needed
    glStencilMask(0x00);
}

void Global::cheap2Copy() {
    bool cheap{ sizeof(glm::vec3) <= 2 * sizeof(void*) };
    std::println("Is glm::vec3 cheap to copy: {} (provided there are no additional setup costs)", cheap);
    cheap = sizeof(glm::vec4) <= 2 * sizeof(void*);
    std::println("Is glm::vec4 cheap to copy: {} (provided there are no additional setup costs)", cheap);
    cheap = sizeof(glm::mat3) <= 2 * sizeof(void*);
    std::println("Is glm::mat3 cheap to copy: {} (provided there are no additional setup costs)", cheap);
    cheap = sizeof(glm::mat4) <= 2 * sizeof(void*);
    std::println("Is glm::mat4 cheap to copy: {} (provided there are no additional setup costs)", cheap);
    cheap = sizeof(Shader) <= 2 * sizeof(void*);
    std::println("Is Shader cheap to copy: {} (provided there are no additional setup costs)", cheap);
    //cheap = sizeof(Texture) <= 2 * sizeof(void*);
    //std::println("Is Texture cheap to copy: {} (provided there are no additional setup costs)", cheap);
}

int Global::init(GLFWwindow* window)
{
    assert(sizeof(int) == sizeof(GLint) && "size of int and GL_INT is not equal"); 
    assert(sizeof(unsigned int) == sizeof(GLuint) && "size of unsigned int and GLuint is not equal");
    assert(sizeof(float) == sizeof(GLfloat) && "size of float and GL_FLOAT is not equal");
    assert(sizeof(unsigned char) == sizeof(GLubyte) && "size of int and GL_INT is not equal");

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_FRAMEBUFFER_SRGB);

    // Debug
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

void Global::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Global::camera.processKeyboard(CameraMovement::FORWARD);
        Global::cameraFlashLight.processKeyboard(CameraMovement::FORWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Global::camera.processKeyboard(CameraMovement::BACKWARD);
        Global::cameraFlashLight.processKeyboard(CameraMovement::BACKWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Global::camera.processKeyboard(CameraMovement::LEFT);
        Global::cameraFlashLight.processKeyboard(CameraMovement::LEFT);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Global::camera.processKeyboard(CameraMovement::RIGHT);
        Global::cameraFlashLight.processKeyboard(CameraMovement::RIGHT);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        Global::camera.processKeyboard(CameraMovement::UP);
        Global::cameraFlashLight.processKeyboard(CameraMovement::UP);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        Global::camera.processKeyboard(CameraMovement::DOWN);
        Global::cameraFlashLight.processKeyboard(CameraMovement::DOWN);
    }
}

#pragma warning( suppress : 4100 )
void Global::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        flashLightOnUpdated = false;

    if (key == GLFW_KEY_K && action == GLFW_PRESS)
        Global::frustumVisible = !frustumVisible;

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
        }
    }

    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            Global::windowsHasMouseFocus = false;
        }
        else if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
        drawOutline = !drawOutline;

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        Global::paused = !Global::paused;

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        Global::debugQuadVisible = !debugQuadVisible;

    if (key == GLFW_KEY_V && action == GLFW_PRESS)
        glfwSwapInterval(0);
}

#pragma warning( suppress : 4100 )
void Global::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Global::windowWidth = width;
    Global::windowHeight = height;
    Global::camera.setAspectRatio((static_cast<float>(Global::windowWidth) / static_cast<float>(Global::windowHeight)));
    Global::glCheckError();
}

#pragma warning( suppress : 4100 )
void Global::cursor_enter_callback(GLFWwindow* window, int entered)
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
void Global::mouse_callback(GLFWwindow* window, double currentXPosIn, double currentYPosIn)
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

    Global::camera.processMouseMovement(xoffset, yoffset);
    Global::cameraFlashLight.processMouseMovement(xoffset, yoffset);
}

#pragma warning( suppress : 4100 )
void Global::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Global::camera.processMouseScroll(static_cast<float>(yoffset));
    Global::cameraFlashLight.processMouseScroll(static_cast<float>(yoffset));
}

#pragma warning( suppress : 4100 )
void APIENTRY Global::glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
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

void Global::getInformation() {
    GLint returnData{};
    std::println("************************************************");
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &returnData);
    std::println("Currently bound VertexArray: {}", returnData);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &returnData);
    std::println("Currently bound ElementBuffer: {}", returnData);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &returnData);
    std::println("Currently bound VertexBuffer: {}", returnData);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &returnData);
    std::println("Currently bound Texture: {}", returnData);
    glGetIntegerv(GL_ACTIVE_TEXTURE, &returnData);
    std::println("Currently active Texture unit: {}", returnData - 33984);
    glGetIntegerv(GL_CURRENT_PROGRAM, &returnData);
    std::println("Currently active program (Shader): {}", returnData);
    std::println("************************************************");
    glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &returnData);
    std::println("maximum supported texture image units: {}", returnData);
    std::println("************************************************");
}