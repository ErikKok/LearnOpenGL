#pragma once

#include "Engine.h"
#include "Global.h"
#include "GlobalEntities.h"

#include "imgui-docking/imgui.h"
#include "imgui-docking/imgui_impl_glfw.h"
#include "imgui-docking/imgui_impl_opengl3.h"

const GLenum G::glCheckError_(const char* file, int line)
{
    GLenum errorCode{ glGetError() };
    while (errorCode != GL_NO_ERROR)
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

void G::glClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

// Takes in full transformation parameters in World space, and outputs model in World space
glm::mat4 G::calculateModelMatrix(glm::vec3 translate, float rotateDegrees, glm::vec3 rotateVec3, glm::vec3 scale)
{
    glm::mat4 model{ 1.0f };
    model = glm::translate(model, translate);
    if (rotateVec3 != glm::vec3(0.0f, 0.0f, 0.0f))
        model = glm::rotate(model, glm::radians(rotateDegrees), rotateVec3);
    if (scale != glm::vec3(0.0f, 0.0f, 0.0f))
        model = glm::scale(model, scale);
    return model;
}

// Takes in full transformation parameters in World space, and outputs model in View space
glm::mat4 G::calculateModelViewMatrix(glm::vec3 translate, float rotateDegrees, glm::vec3 rotateVec3, glm::vec3 scale)
{ 
    return GE::camera.getViewMatrix() * calculateModelMatrix(translate, rotateDegrees, rotateVec3, scale);
}

// see https://stackoverflow.com/questions/49840131/unity-how-to-calculate-a-target-position-with-offset-based-on-the-current-posi
// and https://stackoverflow.com/questions/72095398/translate-objects-relative-to-the-camera-view (I guess I could inverse modelViewMatrix instead, same same...)
void G::applyCameraOffset(Camera* cam, float x, float y, float z) {
    glm::mat4 offsetMatrix{ glm::translate(glm::mat4(1.0f), GE::camera.getRight() * x) };
    offsetMatrix = glm::translate(offsetMatrix, GE::camera.getUp() * y);
    offsetMatrix = glm::translate(offsetMatrix, GE::camera.getFront() * z);

    cam->setFront(GE::camera.getFront());
    cam->setPosition({ offsetMatrix * glm::vec4(GE::camera.getPosition(), 1.0f) });
}

void G::cheap2Copy() {
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

#pragma warning( suppress : 4100 )
void APIENTRY G::glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
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

void G::getInformation() {
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

void G::ImGui() {
    ImGui::Begin("ImGui Window");

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    ImGui::SliderFloat("m_maxJumpSpeed", &GE::player.m_maxJumpSpeed, 0.0f, 2000.0f, "%.1f");
    ImGui::SliderFloat("m_jumpAcceleration", &GE::player.m_jumpAcceleration, 0.0f, 2000.0f, "%.1f");
    ImGui::SliderFloat("m_maxJumpAcceleration", &GE::player.m_maxJumpAcceleration, 0.0f, 2000.0f, "%.1f");
    ImGui::SliderFloat("m_dryFriction", &GE::player.m_dryFriction, 0.0f, 100.0f, "%.1f");

    //ImGui::Text("m_position = %f", GE::camera.m_position);
    //ImGui::Text("m_front = %f", GE::camera.m_front);
    
    // call during physicsloop / split in 2 
    // move to main?

    ImGui::Text("ticksLoop = %d", Engine::ticksLoop);
    ImGui::Text("ticksPhysics = %d", Engine::ticksPhysics);

    ImGui::Text("m_speed = %.2f, %.2f, %.2f", GE::player.m_speed.x, GE::player.m_speed.y, GE::player.m_speed.z);
    ImGui::Text("m_maxCurrentSpeed = %.2f", GE::player.m_maxCurrentSpeed);
    ImGui::Text("m_forwardSpeed = %.2f", GE::player.m_forwardSpeed);
    ImGui::Text("m_acceleration = 000%.2f, 000%.2f, 000%.2f", GE::player.m_acceleration.x, GE::player.m_acceleration.y, GE::player.m_acceleration.z);

    // %d int
    // %g insignificant zeroes to the right of the decimal point are not included

    ImGui::End();
};