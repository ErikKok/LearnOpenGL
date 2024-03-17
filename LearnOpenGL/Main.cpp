#pragma once
#include "BufferSubData.h"
#include "Camera.h"
#include "Data.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexAttribute.h"
#include "UniformBuffer.h"

// External header warning level: 0
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#include <array>
#include <print>

namespace Global {
    // camera
    int windowWidth{ 1920 };
    int windowHeight{ 1080 };
    Camera camera((static_cast<float>(Global::windowWidth) / static_cast<float>(Global::windowHeight)), glm::vec3(8.0f, 1.5f, 8.0f));
    bool windowsHasMouseFocus{ false };

    // timing
    GLfloat deltaTime{ 0.0f };	// time between current frame and last frame
    GLfloat lastFrame{ 0.0f };
    bool paused{ false };
}

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_enter_callback(GLFWwindow* window, int entered);
void mouse_callback(GLFWwindow* window, double currentXPosIn, double currentYPosIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
    assert(sizeof(unsigned int) == sizeof(GLuint) && "size of unsigned int and GLuint is not equal");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    GLFWwindow* window = glfwCreateWindow(Global::windowWidth, Global::windowHeight, "LearnOpenGL", nullptr, nullptr);
    if (!window) {
        std::println("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
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


    UniformBuffer projectionView(0, 2);

    //// SingleCube

    //VertexArray singleCubeVao;
    VertexBuffer cubeVbo(&Data::cube, sizeof(Data::cube));
    //VertexAttributeLayout singleCubeLayout;
    //singleCubeLayout.pushVertexAttributeLayout<float>(3);
    //singleCubeLayout.pushVertexAttributeLayout<float>(3);
    //singleCubeLayout.setVertexAttributeOffset(1, 8);
    ////singleCubeLayout.setVertexStride(32);
    //singleCubeVao.addVertexAttributeLayout(cubeVbo, singleCubeLayout);

    //Shader singleCubeShader("Shaders\\singleCube.shader");
    //singleCubeShader.useShader();
    glm::vec3 objectColorWhite{ 1.0f, 1.0f, 1.0f };
    //singleCubeShader.setVec3("objectColor", objectColorWhite);
    ////singleCubeShader.setVec3("material.ambient", objectColorWhite); // what color the surface reflects under ambient lighting
    ////singleCubeShader.setVec3("material.diffuse", objectColorWhite); // the color of the surface under diffuse lighting
    //singleCubeShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // color of the specular highlight on the surface
    //singleCubeShader.setFloat("material.shininess", 256.0f); // impacts the scattering/radius of the specular highlight
    //// intensity vectors for each of the lighting components
    //singleCubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    //singleCubeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
    //singleCubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // Cube

    VertexArray cubeVao;
    //VertexBuffer cubeVbo(&Data::cube, sizeof(Data::cube));
    VertexAttributeLayout cubeLayout;
    cubeLayout.pushVertexAttributeLayout<float>(3);
    cubeLayout.pushVertexAttributeLayout<float>(2);
    cubeLayout.pushVertexAttributeLayout<float>(3);
    cubeVao.addVertexAttributeLayout(cubeVbo, cubeLayout);

    //Texture texture0("Textures\\container.jpg");
    //Texture texture1("Textures\\awesomeface.png");
    Texture diffuse("Textures\\container2.png"); // 0
    Texture specular("Textures\\container2_specular.png"); // 1

    Shader cubeShader("Shaders\\cube.shader");
    cubeShader.useShader();
    cubeShader.setInt("material.diffuse", 0);
    cubeShader.setInt("material.specular", 1);
    cubeShader.setVec3("objectColor", objectColorWhite);// objectColorCoral 1.0f, 0.5f, 0.31f);
    cubeShader.setFloat("material.shininess", 256.0f);
    cubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    cubeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
    cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // LightCube

    VertexArray lightVao;
    //VertexBuffer lightVbo(&cube, sizeof(cube));
    VertexAttributeLayout lightLayout;
    lightLayout.pushVertexAttributeLayout<float>(3);
    lightLayout.setVertexStride(32);
    lightVao.addVertexAttributeLayout(cubeVbo, lightLayout);

    Shader lightShader("Shaders\\light.shader"); //lightCubeShader
    lightShader.useShader();

    // XYZ

    VertexArray xyzVao;
    VertexBuffer xyzVbo(&Data::xyz, sizeof(Data::xyz));
    VertexAttributeLayout xyzLayout;
    xyzLayout.pushVertexAttributeLayout<float>(3);
    xyzLayout.pushVertexAttributeLayout<float>(3);
    xyzVao.addVertexAttributeLayout(xyzVbo, xyzLayout);

    Shader xyzShader("Shaders\\xyz.shader");

    // Floor

    VertexArray floorVao;
    VertexBuffer floorVbo(&Data::floor, sizeof(Data::floor));
    VertexAttributeLayout floorlayout{};
    floorlayout.pushVertexAttributeLayout<float>(3);
    floorlayout.pushVertexAttributeLayout<float>(3);
    floorlayout.pushVertexAttributeLayout<float>(2);
    floorVao.addVertexAttributeLayout(floorVbo, floorlayout);
    ElementBuffer floorEbo(&Data::floorIndices, sizeof(Data::floorIndices));

    Texture texture2("Textures\\floor.jpg"); // 2

    Shader floorShader("Shaders\\floor.shader");
    floorShader.useShader();
    floorShader.setInt("texture2", 0);

    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        Global::deltaTime = currentFrame - Global::lastFrame;
        Global::lastFrame = currentFrame;
        //std::println("deltaTime: {}ms", Global::deltaTime * 1000);
        //std::println("Position: {}, {}, {}", Global::camera.m_position.x, Global::camera.m_position.y, Global::camera.m_position.z);
        //std::println("Front: {}, {}, {}", Global::camera.m_front.x, Global::camera.m_front.y, Global::camera.m_front.z);

        processInput(window);

        //Global::camera.fakeGravity(Global::deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        BufferSubDataLayout projectionViewLayout{};
        projectionViewLayout.pushUniformBufferSubData(Global::camera.getProjectionMatrix());
        glm::mat4 view{ Global::camera.GetViewMatrix() };
        projectionViewLayout.pushUniformBufferSubData(view);
        projectionView.addUniformBufferSubData(projectionView, projectionViewLayout);

        // Light Source

        glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
        lightPos = glm::vec3((3.0f * sin(glfwGetTime())), 1.8f, (4.5f * cos(glfwGetTime())));
        //std::println("lightPos: {}, {}, {}", lightPos.x, lightPos.y, lightPos.z);

        glm::vec3 lightColor;
        lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0f)) + 0.2f;
        lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7f)) + 0.2f;
        lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3f)) + 0.2f;

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.9f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

        // LightCube

        lightShader.useShader();
        lightVao.bindVertexArray();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        lightShader.setMat4("model", model);
        lightShader.setVec3("lightColor", lightColor);
        lightShader.setVec3("ambient", ambientColor);
        lightShader.setVec3("diffuse", diffuseColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glBindVertexArray(0);

        // Single Cube

        //singleCubeShader.useShader();
        //singleCubeShader.setVec3("lightPos", lightPos);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
        //singleCubeShader.setMat4("model", model);
        //singleCubeVao.bindVertexArray();
        //singleCubeShader.setVec3("light.ambient", ambientColor);
        //singleCubeShader.setVec3("light.diffuse", diffuseColor);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        // XYZ

        xyzShader.useShader();
        xyzVao.bindVertexArray();
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(Data::xyz.size()));
        //glBindVertexArray(0);

        // Cube

        cubeShader.useShader();
        diffuse.bindTexture(0);
        specular.bindTexture(1);
        cubeShader.setVec3("lightPos", lightPos);

        cubeVao.bindVertexArray();
        for (unsigned int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, Data::cubePositions[i]);
            if (i == 2 || i == 5 || i == 8) {
                float angle = 25.0f + (15 * i);
                model = glm::rotate(model, (float)glfwGetTime() * glm::radians(100.0f) * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            }
            cubeShader.setMat4("model", model);
            cubeShader.setVec3("lightPos", lightPos);

            //glm::vec3 diffuseColor2 = lightColor * glm::vec3(0.9f);
            //glm::vec3 ambientColor2 = diffuseColor2 * glm::vec3(0.2f);

            cubeShader.setVec3("light.ambient", ambientColor);
            cubeShader.setVec3("light.diffuse", diffuseColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glBindVertexArray(0);

        // Floor

        floorShader.useShader();
        texture2.bindTexture();
        floorVao.bindVertexArray();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
        model = glm::scale(model, glm::vec3(20.0, 20.0, 20.0));
        floorShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Data::floor.size()), GL_UNSIGNED_INT, 0);

        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
        //for (unsigned int i = 0; i < 32; i++) {
        //    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -0.0f));
        //    
        //    //model = glm::scale(model, glm::vec3(20.0, 20.0, 20.0));
        //    floorShader.setMat4("model", model);
        //    for (unsigned int j = 0; j < 8; j++) {
        //        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -0.0f));

        //        //model = glm::scale(model, glm::vec3(20.0, 20.0, 20.0));
        //        floorShader.setMat4("model", model);
        //        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(floor.size()), GL_UNSIGNED_INT, 0);
        //    }
        //    model = glm::translate(model, glm::vec3(0.0f, -8.0f, -0.0f));
        //    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(floor.size()), GL_UNSIGNED_INT, 0);
        //}

        //glBindVertexArray(0);

        if (!Global::paused) {
            glfwSwapBuffers(window);
        }     
        glfwPollEvents();
    }
    glfwTerminate();
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
}

#pragma warning( suppress : 4100 ) //#pragma warning( default : 4100 )
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Global::windowWidth = width ;
    Global::windowHeight = height;
    Global::camera.setAspectRatio((static_cast<float>(Global::windowWidth) / static_cast<float>(Global::windowHeight)));
    Global::camera.recalculateProjectionMatrix();
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