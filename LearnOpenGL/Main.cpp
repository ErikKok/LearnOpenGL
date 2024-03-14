#pragma once
#include "Camera.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexAttribute.h"
// External header warning level: 0
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#include <array>
#include <iostream>

namespace Global {
    // camera
    Camera camera(glm::vec3(0.0f, 0.15f, 3.0f));
    const int windowWidth{ 1920 };
    const int windowHeight{ 1080 };
    const float aspectRatio{ static_cast<float>(windowWidth) / static_cast<float>(windowHeight) };

    bool windowsHasMouseFocus{ false };

    // timing
    float deltaTime{ 0.0f };	// time between current frame and last frame
    float lastFrame{ 0.0f };
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
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    GLFWwindow* window = glfwCreateWindow(Global::windowWidth, Global::windowHeight, "LearnOpenGL", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << '\n';
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
        std::cout << "Failed to initialize GLAD" << '\n';
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

    // Uniform Buffer Object

    unsigned int uboProjectionView{};
    glGenBuffers(1, &uboProjectionView);
    glBindBuffer(GL_UNIFORM_BUFFER, uboProjectionView);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboProjectionView, 0, 2 * sizeof(glm::mat4));

    glm::mat4 projection{ glm::perspective(glm::radians(Global::camera.getFov()), static_cast<float>(Global::aspectRatio), Global::camera.getNearPlane(), Global::camera.getFarPlane())};
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Cube

    constexpr std::array cube{
    //      X      Y      Z       Texture1/Normal1  Texture2/Normal2    Normal3
            -0.5f, -0.5f, -0.5f,  0.0f,             0.0f,               -1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,             0.0f,               -1.0f,
             0.5f,  0.5f, -0.5f,  1.0f,             1.0f,               -1.0f,
             0.5f,  0.5f, -0.5f,  1.0f,             1.0f,               -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,             1.0f,               -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,             0.0f,               -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,             0.0f,                1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,             0.0f,                1.0f,
             0.5f,  0.5f,  0.5f,  1.0f,             1.0f,                1.0f,
             0.5f,  0.5f,  0.5f,  1.0f,             1.0f,                1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,             1.0f,                1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,             0.0f,                1.0f,

            -0.5f,  0.5f,  0.5f,  1.0f,             0.0f,                0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f,             1.0f,                0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,             1.0f,                0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,             1.0f,                0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,             0.0f,                0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f,             0.0f,                0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,             0.0f,                0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,             1.0f,                0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,             1.0f,                0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,             1.0f,                0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,             0.0f,                0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,             0.0f,                0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f,             1.0f,                0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,             1.0f,                0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,             0.0f,                0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,             0.0f,                0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,             0.0f,                0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,             1.0f,                0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,             1.0f,                0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,             1.0f,                0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,             0.0f,                0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,             0.0f,                0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,             0.0f,                0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,             1.0f,                0.0f,
    };

    //constexpr std::array cube2{
    //    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 0.0f,  0.0f, -1.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 0.0f,  0.0f, -1.0f,
    //     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 0.0f,  0.0f, -1.0f,
    //     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 0.0f,  0.0f, -1.0f,
    //    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 0.0f,  0.0f, -1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 0.0f,  0.0f, -1.0f,

    //    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 0.0f,  0.0f, 1.0f,
    //     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 0.0f,  0.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 0.0f,  0.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 0.0f,  0.0f, 1.0f,
    //    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 0.0f,  0.0f, 1.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 0.0f,  0.0f, 1.0f,

    //    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f, 1.0f,  0.0f,  0.0f,
    //    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f, 1.0f,  0.0f,  0.0f,
    //    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f, 1.0f,  0.0f,  0.0f,
    //    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f, 1.0f,  0.0f,  0.0f,
    //    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f, 1.0f,  0.0f,  0.0f,
    //    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f, 1.0f,  0.0f,  0.0f,

    //     0.5f,  0.5f,  0.5f,  1.0f,  0.0f, 1.0f,  0.0f,  0.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f,  0.0f, 1.0f,  0.0f,  0.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f,  0.0f, 1.0f,  0.0f,  0.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f,  0.0f, 1.0f,  0.0f,  0.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f,  0.0f, 1.0f,  0.0f,  0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f,  0.0f, 1.0f,  0.0f,  0.0f,

    //    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, -1.0f,  0.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, -1.0f,  0.0f,
    //     0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f, -1.0f,  0.0f,
    //     0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f, -1.0f,  0.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f, -1.0f,  0.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, -1.0f,  0.0f,

    //    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  1.0f,  0.0f,
    //     0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  1.0f,  0.0f,
    //     0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 0.0f,  1.0f,  0.0f,
    //     0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 0.0f,  1.0f,  0.0f,
    //    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 0.0f,  1.0f,  0.0f,
    //    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  1.0f,  0.0f
    //};

    constexpr glm::vec3 cubePositions[] = {
        glm::vec3(1.0f,  0.5f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    VertexArray cubeVao;
    VertexBuffer cubeVbo(&cube, sizeof(cube));
    VertexAttributeLayout cubeLayout;
    cubeLayout.pushVertexAttributeLayout<float>(3);
    cubeLayout.pushVertexAttributeLayout<float>(2);
    cubeLayout.pushVertexAttributeLayout<float>(3);
    cubeLayout.setVertexOffset(1, 0);
    cubeLayout.setVertexOffset(2, 12);
    cubeLayout.setVertexStride(24);

    cubeVao.addVertexAttributeLayout(cubeVbo, cubeLayout);

    //unsigned int vao0{}; // vertex array object
    //glGenVertexArrays(1, &vao0);
    //glBindVertexArray(vao0);

    //unsigned int vbo0{}; // vertex buffer object
    //glGenBuffers(1, &vbo0);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo0);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), &vertices0, GL_STATIC_DRAW);

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(2);

    //glBindVertexArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Cube Texture

    Texture texture0("Textures\\container.jpg");
    Texture texture1("Textures\\awesomeface.png", 1);

    // Cube Shader
    Shader ourShader("Shaders\\cube.txt"); //lightingShader
    ourShader.useShader();
    ourShader.setInt("texture0", 0);
    ourShader.setInt("texture1", 1);
    ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    ourShader.setVec3("lightPos", 2.0f, 2.0f, 1.0f);

    // Light

    VertexArray lightVao;
    VertexAttributeLayout lightLayout;
    lightLayout.pushVertexAttributeLayout<float>(3);
    lightLayout.setVertexStride(24);
    lightVao.addVertexAttributeLayout(cubeVbo, lightLayout);

    Shader lightShader("Shaders\\light.txt"); //lightCubeShader
    lightShader.useShader();

    // XYZ

    constexpr std::array xyz{ // Hex
        // Positions            // Colors        
         -10.0f,   0.0f,  0.0f,   1.0f, 0.0f, 0.0f,
          10.0f,   0.0f,  0.0f,   1.0f, 0.0f, 0.0f,
          0.0f,  -10.0f,  0.0f,   0.0f, 1.0f, 0.0f,
          0.0f,   10.0f,  0.0f,   0.0f, 1.0f, 0.0f,
          0.0f,    0.0f, -10.0f,  0.0f, 0.0f, 1.0f,
          0.0f,    0.0f,  10.0f,  0.0f, 0.0f, 1.0f,
    };

    VertexArray xyzVao;
    VertexBuffer xyzVbo(&xyz, sizeof(xyz));
    VertexAttributeLayout xyzLayout;
    xyzLayout.pushVertexAttributeLayout<float>(3);
    xyzLayout.pushVertexAttributeLayout<float>(3);
    xyzVao.addVertexAttributeLayout(xyzVbo, xyzLayout);

    Shader xyzShader("Shaders\\xyz.txt");

    // Floor

    constexpr std::array floor{ // Hex
    // Positions            // Colors          // Texture
      0.0f,   0.0f,  0.0f,  0.2f, 0.2f, 0.2f,  0.5f, 0.5f,  // 0
      0.0f,   1.0f,  0.0f,  1.0f, 0.0f, 0.0f,  0.5f, 1.0f,  // 1
      0.67f,  0.50f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.75f, // 2
      0.67f, -0.50f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.25f, // 3   
      0.0f,  -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,  0.5f, 0.0f,  // 4
     -0.67f, -0.50f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.25f, // 5
     -0.67f,  0.50f, 0.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.75f, // 6
    };

    constexpr std::array floorIndices{
     0u, 1u, 2u,
     0u, 2u, 3u,
     0u, 3u, 4u,
     0u, 4u, 5u,
     0u, 5u, 6u,
     0u, 6u, 1u,
    };

    VertexArray floorVao;
    VertexBuffer floorVbo(&floor, sizeof(floor));
    VertexAttributeLayout floorlayout{};
    floorlayout.pushVertexAttributeLayout<float>(3);
    floorlayout.pushVertexAttributeLayout<float>(3);
    floorlayout.pushVertexAttributeLayout<float>(2);
    floorVao.addVertexAttributeLayout(floorVbo, floorlayout);
    ElementBuffer floorEbo(&floorIndices, sizeof(floorIndices));

    Texture texture2("Textures\\floor.jpg");

    Shader floorShader("Shaders\\floor.txt");
    floorShader.useShader();
    floorShader.setInt("texture2", 0);

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        Global::deltaTime = currentFrame - Global::lastFrame;
        Global::lastFrame = currentFrame;
        //std::cout << "deltaTime: " << Global::deltaTime * 1000 << "ms" << '\n';
        std::cout << "Position: " << Global::camera.m_position.x << ", " << Global::camera.m_position.y << ", " << Global::camera.m_position.z << ", " << '\n';
        std::cout << "Front: " << Global::camera.m_front.x << ", " << Global::camera.m_front.y << ", " << Global::camera.m_front.z << ", " << '\n';

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = glm::perspective(glm::radians(Global::camera.getFov()), (Global::aspectRatio), Global::camera.getNearPlane(), Global::camera.getFarPlane());
        glBindBuffer(GL_UNIFORM_BUFFER, uboProjectionView);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        Global::camera.fakeGravity(Global::deltaTime);

        glm::mat4 view{ Global::camera.GetViewMatrix() };

        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        // XYZ
        
        xyzShader.useShader();
        xyzVao.bindVertexArray();
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(xyz.size()));
        glBindVertexArray(0);

        // Light

        lightShader.useShader();
        lightVao.bindVertexArray();

        glm::mat4 model{ glm::mat4(1.0f) };
        model = glm::translate(model, glm::vec3(2.0f, 2.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
        lightShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Cube

        ourShader.useShader();
        texture0.bindTexture();
        texture1.bindTexture(1);
        ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("lightPos", 2.0f, 2.0f, 1.0f);
        
        cubeVao.bindVertexArray();
        for (unsigned int i = 0; i < 10; i++)
        {
            //glm::mat4 model{ glm::mat4(1.0f) };
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(100.0f) * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            //if (i == 0) {
            //    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
            //}
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        
        // Floor

        floorShader.useShader();
        texture2.bindTexture();
        floorVao.bindVertexArray();
        //glm::mat4 model{ glm::mat4(1.0f) };
        model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 1.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
        model = glm::scale(model, glm::vec3(20.0, 20.0, 20.0));
        floorShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(floor.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //glDeleteVertexArrays(1, &vao0);
    //glDeleteBuffers(1, &vbo0);
    //glDeleteVertexArrays(1, &vao1);
    //glDeleteBuffers(1, &vbo1);
    //glDeleteVertexArrays(1, &vao2);
    //glDeleteBuffers(1, &vbo2);
    //glDeleteBuffers(1, &ebo2);

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
}

#pragma warning( suppress : 4100 ) //#pragma warning( default : 4100 )
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------\n";
    std::cout << "Debug message (" << id << "): \n";

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << '\n';

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << '\n';

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << '\n';

    std::cout << '\n';
}