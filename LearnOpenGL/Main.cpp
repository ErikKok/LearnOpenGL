#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <array>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int width = 1600;
    const int height = 1200;

    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << '\n';
        return -1;
    }

    Shader ourShader("3.3.shader.vs", "3.3.shader.fs");

    // Vertex data

    //constexpr std::array vertices0{ // /\/\
    // -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    // -0.25f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    //  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    //  0.25f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
    //  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    //};

    //constexpr std::array indices0{
    // 0u, 1u, 2u,  // first Triangle
    // 2u, 3u, 4u   // second Triangle
    //};

    constexpr std::array vertices0{
      0.0f,   0.0f,  0.0f,  0.0f, 0.0f, 0.0f,
      0.0f,   0.5f,  0.0f,  1.0f, 0.0f, 0.0f,
      0.33f,  0.25f, 0.0f,  0.0f, 1.0f, 0.0f,
      0.33f, -0.25f, 0.0f,  0.0f, 0.0f, 1.0f,
      0.0f,  -0.5f,  0.0f,  0.0f, 1.0f, 1.0f,
     -0.33f, -0.25f, 0.0f,  1.0f, 1.0f, 0.0f,
     -0.33f,  0.25f, 0.0f,  1.0f, 0.0f, 1.0f,
      0.0f,   0.5f,  0.0f,  1.0f, 0.0f, 0.0f,
    };

    constexpr std::array indices0{
     0u, 1u, 2u,
     0u, 2u, 3u,
     0u, 3u, 4u,
     0u, 4u, 5u,
     0u, 5u, 6u,
     0u, 6u, 7u,
    };

    unsigned int vao0; // vertex array object
    glGenVertexArrays(1, &vao0);
    glBindVertexArray(vao0);

    unsigned int vbo0; // vertex buffer object
    glGenBuffers(1, &vbo0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), &vertices0, GL_STATIC_DRAW);

    unsigned int ebo0; // element buffer object
    glGenBuffers(1, &ebo0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices0), &indices0, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    ourShader.use();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
   
        glBindVertexArray(vao0);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertices0.size()), GL_UNSIGNED_INT, 0);

        //float xOffset{ 0.0 };
        //for (int i = 0; i < 3; i++)
        //{
        //    ourShader.setFloat("xOffset", xOffset);
        //    xOffset += 0.1f;
        //    glDrawElements(GL_TRIANGLES, 49, GL_UNSIGNED_INT, 0);
        //}

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao0);
    glDeleteBuffers(1, &vbo0);
    glDeleteBuffers(1, &ebo0);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

#pragma warning( suppress : 4100 ) //#pragma warning( default : 4100 )
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}