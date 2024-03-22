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

    Texture diffuse("Textures\\container2.png"); // 0
    Texture specular("Textures\\container2_specular.png"); // 1
    Texture emission("Textures\\matrix.jpg"); // 3


    // Shader spotlight
    //Shader spotlight("Shaders\\spotlight.shader");
    // Shader multiLight
    Shader multiLight("Shaders\\multiLight.shader");







    multiLight.useShader();
    multiLight.setInt("material.diffuse", 0); // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    multiLight.setInt("material.specular", 1);
    multiLight.setInt("material.emission", 3);
    multiLight.setFloat("material.shininess", 256.0f);
    glm::vec3 lightPos(0.0f, 5.2f, 3.0f);
    multiLight.setVec3("spotLight.position", lightPos);
    glm::vec3 lightDirection{ 0.0f, -1.0f, 0.0f };
    multiLight.setVec3("spotLight.direction", lightDirection);
    float cutOff{ glm::cos(glm::radians(48.5f)) };
    multiLight.setFloat("spotLight.cutOff", cutOff);
    float outerCutOff{ glm::cos(glm::radians(52.5f)) };
    multiLight.setFloat("spotLight.outerCutOff", outerCutOff);
    glm::vec3 lightColor{ 1.0f, 0.1f, 0.1f };
    multiLight.setVec3("spotLight.ambientStrength", (lightColor * glm::vec3(0.1f)));
    multiLight.setVec3("spotLight.diffuseStrength", (lightColor * glm::vec3(0.5f)));
    multiLight.setVec3("spotLight.specularStrength", 0.5f, 0.5f, 0.5f);
    multiLight.setFloat("spotLight.constant", 1.0f);
    multiLight.setFloat("spotLight.linear", 0.045f);
    multiLight.setFloat("spotLight.quadratic", 0.0075f); 

    glm::vec3 dirLightDirection{ 0.2f, 0.8f, 0.2f };
    multiLight.setVec3("dirLight.direction", dirLightDirection);
    glm::vec3 dirLightColor(0.0f, 1.0f, 0.2f);
    multiLight.setVec3("dirLight.ambient", (dirLightColor * glm::vec3(0.1f)));
    multiLight.setVec3("dirLight.diffuse", (dirLightColor * glm::vec3(0.8f)));
    multiLight.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  8.2f,  2.0f),
        glm::vec3( 2.3f,  3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 15.0f,  1.2f, -3.0f)
    };

    multiLight.setVec3("pointLights[0].position", pointLightPositions[0]);
    glm::vec3 pointLightColor(1.0f, 1.0f, 1.0f);
    multiLight.setVec3("pointLights[0].ambient", (pointLightColor * glm::vec3(0.1f)));
    multiLight.setVec3("pointLights[0].diffuse", (pointLightColor * glm::vec3(1.0f)));
    multiLight.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    multiLight.setFloat("pointLights[0].constant", 1.0f);
    multiLight.setFloat("pointLights[0].linear", 0.09f);
    multiLight.setFloat("pointLights[0].quadratic", 0.032f);

    multiLight.setVec3("pointLights[1].position", pointLightPositions[1]);
    //glm::vec3 pointLightColor(1.0f, 1.0f, 1.0f);
    multiLight.setVec3("pointLights[1].ambient", (pointLightColor* glm::vec3(0.1f)));
    multiLight.setVec3("pointLights[1].diffuse", (pointLightColor* glm::vec3(1.0f)));
    multiLight.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    multiLight.setFloat("pointLights[1].constant", 1.0f);
    multiLight.setFloat("pointLights[1].linear", 0.09f);
    multiLight.setFloat("pointLights[1].quadratic", 0.032f);

    multiLight.setVec3("pointLights[2].position", pointLightPositions[2]);
    //glm::vec3 pointLightColor(1.0f, 1.0f, 1.0f);
    multiLight.setVec3("pointLights[2].ambient", (pointLightColor* glm::vec3(0.1f)));
    multiLight.setVec3("pointLights[2].diffuse", (pointLightColor* glm::vec3(1.0f)));
    multiLight.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    multiLight.setFloat("pointLights[2].constant", 1.0f);
    multiLight.setFloat("pointLights[2].linear", 0.09f);
    multiLight.setFloat("pointLights[2].quadratic", 0.032f);

    multiLight.setVec3("pointLights[3].position", pointLightPositions[3]);
    //glm::vec3 pointLightColor(1.0f, 1.0f, 1.0f);
    multiLight.setVec3("pointLights[3].ambient", (pointLightColor* glm::vec3(0.1f)));
    multiLight.setVec3("pointLights[3].diffuse", (pointLightColor* glm::vec3(1.0f)));
    multiLight.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    multiLight.setFloat("pointLights[3].constant", 1.0f);
    multiLight.setFloat("pointLights[3].linear", 0.09f);
    multiLight.setFloat("pointLights[3].quadratic", 0.032f);

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
    VertexBuffer floorVbo(&Data::floor2, sizeof(Data::floor2));
    VertexAttributeLayout floorlayout{};
    floorlayout.pushVertexAttributeLayout<float>(3);
    floorlayout.pushVertexAttributeLayout<float>(2);
    floorlayout.pushVertexAttributeLayout<float>(3);
    floorVao.addVertexAttributeLayout(floorVbo, floorlayout);
    ElementBuffer floorEbo(&Data::floorIndices, sizeof(Data::floorIndices));

    Texture texture2("Textures\\floor.jpg");

    //Shader floorShader("Shaders\\floor.shader");
    //floorShader.useShader();
    //floorShader.setInt("texture2", 2);

    Global::glCheckError();
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        Global::deltaTime = currentFrame - Global::lastFrame;
        Global::lastFrame = currentFrame;
        //std::println("deltaTime: {}ms", Global::deltaTime * 1000);
        //std::println("Position: {}, {}, {}", Global::camera.m_position.x, Global::camera.m_position.y, Global::camera.m_position.z);
        //std::println("Front: {}, {}, {}", Global::camera.m_front.x, Global::camera.m_front.y, Global::camera.m_front.z);

        Global::processInput(window);

        //Global::camera.fakeGravity(Global::deltaTime);

        BufferSubDataLayout projectionViewLayout{};
        projectionViewLayout.pushUniformBufferSubData(Global::camera.getProjectionMatrix());
        glm::mat4 view{ Global::camera.GetViewMatrix() };
        projectionViewLayout.pushUniformBufferSubData(view);
        projectionView.addUniformBufferSubData(projectionView, projectionViewLayout);



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

        multiLight.useShader();
        lightPos = glm::vec3((3.0f * sin(glfwGetTime())), 6.5f, (4.5f * cos(glfwGetTime())));
        //std::println("lightPos: {}, {}, {}", lightPos.x, lightPos.y, lightPos.z);
        multiLight.setVec3("spotLight.position", lightPos);
        lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0f)) + 0.2f;
        lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7f)) + 0.2f;
        lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3f)) + 0.2f;
        multiLight.setVec3("spotLight.ambientStrength", (lightColor * glm::vec3(0.1f)));
        multiLight.setVec3("spotLight.diffuseStrength", (lightColor * glm::vec3(0.5f)));
        diffuse.bindTexture(0);
        specular.bindTexture(1);
        emission.bindTexture(3);
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
            //if (i == 9) {
            //    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            //    model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
            //}
            multiLight.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Floor

        //spotlight.useShader();
        //texture2.bindTexture(2);
        floorVao.bindVertexArray();
        model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
        model = glm::scale(model, glm::vec3(25.0, 25.0, 1.0));
        multiLight.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Data::floor2.size()), GL_UNSIGNED_INT, 0);

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