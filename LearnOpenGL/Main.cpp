#pragma once
#include "BufferSubData.h"
#include "Camera.h"
#include "Data.h"
#include "ElementBuffer.h"
#include "Global.h"
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

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    GLFWwindow* window = glfwCreateWindow(Global::windowWidth, Global::windowHeight, "LearnOpenGL", nullptr, nullptr);
    if (!window) {
        std::println("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    if (Global::init(window) == -1)
        return 1;
 
    Global::glCheckError();

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
    Texture emission("Textures\\matrix.jpg"); // 3

    Shader cubeShader("Shaders\\cube.shader");
    cubeShader.useShader();

    cubeShader.setInt("material.diffuse", 0); // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    cubeShader.setInt("material.specular", 1);
    cubeShader.setInt("material.emission", 3);
    //cubeShader.setVec3("objectColor", objectColorWhite);// objectColorCoral 1.0f, 0.5f, 0.31f);
    cubeShader.setFloat("material.shininess", 256.0f);
    cubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    cubeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
    cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    //sun cubeShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);

    cubeShader.setFloat("light.constant", 1.0f);
    cubeShader.setFloat("light.linear", 0.09f);
    cubeShader.setFloat("light.quadratic", 0.032f);

    //cubeShader.setVec3("light.position", Global::camera.getPosition());
    //cubeShader.setVec3("light.direction", Global::camera.getFront());
 


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

    //VertexArray floorVao;
    //VertexBuffer floorVbo(&Data::floor, sizeof(Data::floor));
    //VertexAttributeLayout floorlayout{};
    //floorlayout.pushVertexAttributeLayout<float>(3);
    //floorlayout.pushVertexAttributeLayout<float>(3);
    //floorlayout.pushVertexAttributeLayout<float>(2);
    //floorVao.addVertexAttributeLayout(floorVbo, floorlayout);
    //ElementBuffer floorEbo(&Data::floorIndices, sizeof(Data::floorIndices));

    //Texture texture2("Textures\\floor.jpg");

    //Shader floorShader("Shaders\\floor.shader");
    //floorShader.useShader();
    //floorShader.setInt("texture2", 2);

    Global::glCheckError();
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        Global::deltaTime = currentFrame - Global::lastFrame;
        Global::lastFrame = currentFrame;
        //std::println("deltaTime: {}ms", Global::deltaTime * 1000);
        //std::println("Position: {}, {}, {}", Global::camera.m_position.x, Global::camera.m_position.y, Global::camera.m_position.z);
        //std::println("Front: {}, {}, {}", Global::camera.m_front.x, Global::camera.m_front.y, Global::camera.m_front.z);

        Global::processInput(window);

        //Global::camera.fakeGravity(Global::deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        BufferSubDataLayout projectionViewLayout{};
        projectionViewLayout.pushUniformBufferSubData(Global::camera.getProjectionMatrix());
        glm::mat4 view{ Global::camera.GetViewMatrix() };
        projectionViewLayout.pushUniformBufferSubData(view);
        projectionView.addUniformBufferSubData(projectionView, projectionViewLayout);

        // Light Source

        glm::vec3 lightPos(0.0f, 3.2f, -1.0f);
        lightPos = glm::vec3((3.0f * sin(glfwGetTime())), 1.5f, (4.5f * cos(glfwGetTime())));
        //std::println("lightPos: {}, {}, {}", lightPos.x, lightPos.y, lightPos.z);

        glm::vec3 lightColor{ 1.0f, 0.8f, 0.6f };
        //lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0f)) + 0.2f;
        //lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7f)) + 0.2f;
        //lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3f)) + 0.2f;

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
        //lightShader.setVec3("ambient", ambientColor);
        //lightShader.setVec3("diffuse", diffuseColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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

        // Cube

        cubeShader.useShader();
        diffuse.bindTexture(0);
        specular.bindTexture(1);
        emission.bindTexture(3);
        cubeShader.setVec3("lightPos", lightPos);
        cubeShader.setVec3("light.position", lightPos);
        cubeShader.setVec3("light.direction", glm::vec3(0.0f, 0.0f, -1.0f));
        cubeShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        cubeShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

        cubeVao.bindVertexArray();
        for (unsigned int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, Data::cubePositions[i]);
            if (i == 2 || i == 5 || i == 8) {
                float angle = 25.0f + (15 * i);
                model = glm::rotate(model, (float)glfwGetTime() * glm::radians(100.0f) * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            }
            if (i == 3) {
                model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -3.0f));
                model = glm::scale(model, glm::vec3(20.0, 20.0, 1.0));
            }
            if (i == 9) {
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
                model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
            }
            cubeShader.setMat4("model", model);
            cubeShader.setVec3("lightPos", lightPos);

            //glm::vec3 diffuseColor2 = lightColor * glm::vec3(0.9f);
            //glm::vec3 ambientColor2 = diffuseColor2 * glm::vec3(0.2f);

            cubeShader.setVec3("light.ambient", ambientColor);
            cubeShader.setVec3("light.diffuse", diffuseColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Floor

        //floorShader.useShader();
        //texture2.bindTexture(2);
        //floorVao.bindVertexArray();
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
        //model = glm::scale(model, glm::vec3(20.0, 20.0, 20.0));
        //floorShader.setMat4("model", model);
        //glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Data::floor.size()), GL_UNSIGNED_INT, 0);

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

        if (!Global::paused) {
            glfwSwapBuffers(window);
        }     
        glfwPollEvents();
    }
    Global::glCheckError();
    glfwTerminate();
    Global::glClearError(); // TODO glfwTerminate() produce errors
    Global::glCheckError();
    return 0;
}