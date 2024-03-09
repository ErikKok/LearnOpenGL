#pragma once
#include "Shader.h"
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int windowWidth = 1200;
    const int windowHeight = 900;

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", nullptr, nullptr);
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

    constexpr std::array vertices0{ // Hex
    // Positions            // Colors          // Texture
      0.0f,   0.0f,  0.0f,  0.0f, 0.0f, 0.0f,  0.5f, 0.5f,  // 0
      0.0f,   1.0f,  0.0f,  1.0f, 0.0f, 0.0f,  0.5f, 1.0f,  // 1
      0.67f,  0.50f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.75f, // 2
      0.67f, -0.50f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.25f, // 3   
      0.0f,  -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,  0.5f, 0.0f,  // 4
     -0.67f, -0.50f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.25f, // 5
     -0.67f,  0.50f, 0.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.75f, // 6
    };

    constexpr std::array indices0{
     0u, 1u, 2u,
     0u, 2u, 3u,
     0u, 3u, 4u,
     0u, 4u, 5u,
     0u, 5u, 6u,
     0u, 6u, 1u,
    };

    unsigned int vbo0; // vertex buffer object
    glGenBuffers(1, &vbo0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), &vertices0, GL_STATIC_DRAW);

    unsigned int vao0; // vertex array object
    glGenVertexArrays(1, &vao0);
    glBindVertexArray(vao0);

    unsigned int ebo0; // element buffer object
    glGenBuffers(1, &ebo0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices0), &indices0, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned char* textureData{};
    int textureWidth{};
    int textureHeight{};
    int textureNrChannels{};

    unsigned int texture0;
    glGenTextures(1, &texture0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    textureData = stbi_load("container.jpg", &textureWidth, &textureHeight, &textureNrChannels, 0);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << '\n';
    }    
    //stbi_image_free(textureData);

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    textureData = stbi_load("awesomeface.png", &textureWidth, &textureHeight, &textureNrChannels, 0);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << '\n';
    }
    stbi_image_free(textureData);

    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.id, "texture0"), 0);
    glUniform1i(glGetUniformLocation(ourShader.id, "texture1"), 1);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Matrix
        glm::mat4 trans{ glm::mat4(1.0f) };
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(0.5, -0.5, 0.0));

        glUniformMatrix4fv(glGetUniformLocation(ourShader.id, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
   
        glBindVertexArray(vao0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertices0.size()), GL_UNSIGNED_INT, 0);

        // Matrix
        //glm::mat4 trans{ glm::mat4(1.0f) };
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(0.5, -0.5, 0.0));

        glUniformMatrix4fv(glGetUniformLocation(ourShader.id, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertices0.size()), GL_UNSIGNED_INT, 0);

        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        trans = glm::scale(trans, glm::vec3(static_cast<float>(sin(glfwGetTime())), static_cast<float>(sin(glfwGetTime())), static_cast<float>(sin(glfwGetTime()))));

        glUniformMatrix4fv(glGetUniformLocation(ourShader.id, "transform"), 1, GL_FALSE, &trans[0][0]);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertices0.size()), GL_UNSIGNED_INT, 0);

        //float mixAmount{ 0.0 };
        //for (int i = 0; i < 100; i++)
        //{
        //    ourShader.setFloat("mixAmount", mixAmount);
        //    mixAmount += 0.01f;
        //    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertices0.size()), GL_UNSIGNED_INT, 0);
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