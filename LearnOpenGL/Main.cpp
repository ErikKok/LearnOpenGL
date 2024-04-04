#pragma once
#include "BufferSubData.h"
#include "Camera.h"
#include "Data.h"
#include "ElementBuffer.h"
#include "Global.h"
#include "Model.h"
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
//#include <filesystem> // for filesystem
#include <print>

int main()
{
    std::println("START Main");

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

    if (Global::init(window) == -1) {
        std::println("Failed to initialize");
        glfwTerminate();
        return -1;
    }
 
    Global::glCheckError();

    UniformBuffer projectionView(0, 2);

    /////////////////////////////////////
    ////// XYZ //////////////////////////
    /////////////////////////////////////

    VertexArray xyzVao;
    VertexBuffer xyzVbo(&Data::xyz, sizeof(Data::xyz));
    VertexAttributeLayout xyzLayout;
    xyzLayout.pushVertexAttributeLayout<float>(3);
    xyzLayout.pushVertexAttributeLayout<float>(3);
    xyzVao.addVertexAttributeLayout(xyzVbo, xyzLayout);

    Shader xyzShader("Shaders\\xyz.shader");

    /////////////////////////////////////
    ////// Lights ///////////////////////
    /////////////////////////////////////

    Shader multiLight("Shaders\\multiLight.shader");

    multiLight.useShader();
    // Material
    multiLight.setInt("material.diffuse1", 0);
    multiLight.setInt("material.specular1", 1);
    multiLight.setInt("material.emission", 3);
    multiLight.setFloat("material.shininess", 256.0f);
    glm::vec3 lightPos(0.0f, 5.2f, 3.0f);

    // DirLight
    //glm::vec3 dirLightDirection{ 0.1f, 0.8f, 0.1f };
    //multiLight.setVec3("dirLight.direction", dirLightDirection); // world space
    glm::vec3 dirLightColor(1.0f, 1.0f, 0.0f);
    multiLight.setVec3("dirLight.diffuse", (dirLightColor * glm::vec3(0.2f)));

    // SpotLight
    //multiLight.setVec3("spotLight.position", lightPos); // world space

    float spotLightCutOff{ glm::cos(glm::radians(48.5f)) };
    //multiLight.setFloat("spotLight.cutOff", spotLightCutOff);
    float spotLightOuterCutOff{ glm::cos(glm::radians(52.5f)) };
    multiLight.setFloat("spotLight.outerCutOff", spotLightOuterCutOff);
    float spotLightEpsilon{ spotLightCutOff - spotLightOuterCutOff };
    multiLight.setFloat("spotLight.epsilon", spotLightEpsilon);
    glm::vec3 spotLightColor{ 1.0f, 0.1f, 0.1f };
    multiLight.setVec3("spotLight.diffuse", (spotLightColor));
    multiLight.setFloat("spotLight.emission", 0.0f);
    multiLight.setFloat("spotLight.constant", 1.0f);
    multiLight.setFloat("spotLight.linear", 0.045f);
    multiLight.setFloat("spotLight.quadratic", 0.0075f); 

    // PointLight
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  11.2f,  2.0f),
        glm::vec3( 2.3f,  3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, 12.0f),
        glm::vec3( 15.0f,  1.2f, -3.0f)
    };

    glm::vec3 pointLightColors[] = {
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f)
    };

    multiLight.setInt("pointLightsCount", 4); // std::size(pointLightPositions)
    //multiLight.setVec3("pointLights[0].position", pointLightPositions[0]); // World space
    multiLight.setVec3("pointLights[0].diffuse", pointLightColors[0]);
    multiLight.setFloat("pointLights[0].constant", 1.0f);
    multiLight.setFloat("pointLights[0].linear", 0.09f);
    multiLight.setFloat("pointLights[0].quadratic", 0.032f);

    //multiLight.setVec3("pointLights[1].position", pointLightPositions[1]);
    multiLight.setVec3("pointLights[1].diffuse", pointLightColors[1]);
    multiLight.setFloat("pointLights[1].constant", 1.0f);
    multiLight.setFloat("pointLights[1].linear", 0.09f);
    multiLight.setFloat("pointLights[1].quadratic", 0.032f);

    //multiLight.setVec3("pointLights[2].position", pointLightPositions[2]);
    multiLight.setVec3("pointLights[2].diffuse", pointLightColors[2]);
    multiLight.setFloat("pointLights[2].constant", 1.0f);
    multiLight.setFloat("pointLights[2].linear", 0.09f);
    multiLight.setFloat("pointLights[2].quadratic", 0.032f);

    //multiLight.setVec3("pointLights[3].position", pointLightPositions[3]);
    multiLight.setVec3("pointLights[3].diffuse", pointLightColors[3]);
    multiLight.setFloat("pointLights[3].constant", 1.0f);
    multiLight.setFloat("pointLights[3].linear", 0.09f);
    multiLight.setFloat("pointLights[3].quadratic", 0.032f);

    // FlashLight
    flashLight = &multiLight;
    multiLight.setBool("flashLight.on", false);
    float flashLightCutOff{ glm::cos(glm::radians(8.5f)) };
    float flashLightOuterCutOff{ glm::cos(glm::radians(12.5f)) };
    multiLight.setFloat("flashLight.outerCutOff", flashLightOuterCutOff);
    float flashLightEpsilon{ flashLightCutOff - flashLightOuterCutOff };
    multiLight.setFloat("flashLight.epsilon", flashLightEpsilon);
    glm::vec3 flashlightColor{ 1.0f, 1.0f, 1.0f };
    multiLight.setVec3("flashLight.diffuse", (flashlightColor));
    multiLight.setFloat("flashLight.emission", 0.8f);
    multiLight.setFloat("flashLight.constant", 1.0f);
    multiLight.setFloat("flashLight.linear", 0.045f);
    multiLight.setFloat("flashLight.quadratic", 0.0075f);
    
    /////////////////////////////////////
    ////// LightCube ////////////////////
    /////////////////////////////////////

    VertexArray lightVao;
    VertexBuffer cubeVbo(&Data::cube, sizeof(Data::cube));
    VertexAttributeLayout lightLayout;
    lightLayout.pushVertexAttributeLayout<float>(3);
    lightLayout.setVertexStride(32);
    lightVao.addVertexAttributeLayout(cubeVbo, lightLayout);

    Shader lightShader("Shaders\\light.shader"); //lightCubeShader
    lightShader.useShader();

    /////////////////////////////////////
    ////// Cubes ////////////////////////
    /////////////////////////////////////

    VertexArray cubeVao;
    VertexAttributeLayout cubeLayout;
    cubeLayout.pushVertexAttributeLayout<float>(3);
    cubeLayout.pushVertexAttributeLayout<float>(2);
    cubeLayout.pushVertexAttributeLayout<float>(3);
    cubeVao.addVertexAttributeLayout(cubeVbo, cubeLayout);

    ElementBuffer cubeTestEBO(&Data::cubeIndices, sizeof(Data::cubeIndices));

    Texture diffuse("Textures\\container2.png"); // 0
    Texture specular("Textures\\container2_specular.png"); // 1
    Texture emission("Textures\\matrix.jpg"); // 3

    /////////////////////////////////////
    ////// Mesh ////////////////////////
    /////////////////////////////////////

    //Mesh mesh;
    //mesh.m_indices = Data::cubeIndices;
    //mesh.m_positions = ;


    Shader ourModelShader("Shaders\\multiLight.shader");
    Model ourModel("Backpack/backpack.obj");

    //Model ourModel("FinalBaseMesh.obj"); 






    //VertexArray cubeTest;
    //VertexBuffer cubeTestVBO(&Data::cube, sizeof(Data::cube));
    //VertexAttributeLayout cubeTestLayout{};
    //cubeTestLayout.pushVertexAttributeLayout<float>(3); // cubeCoords
    //cubeTestLayout.pushVertexAttributeLayout<float>(2); // cubeTexCoords
    //cubeTestLayout.pushVertexAttributeLayout<float>(3); // cubeNormals
    ////ElementBuffer cubeTestEBO(&Data::cubeIndices, sizeof(Data::cubeIndices));
    ////Texture texture2("Textures\\floor.jpg");
    //Shader cubeTestShader("Shaders\\cubeTest.shader");
    //cubeTestShader.useShader();
    ////cubeTestShader.setInt("texture2", 2); // name vd texture moet gelijk zijn aan name vd sampler2d

    //glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    //cubeTestShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    //cubeTestShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    //cubeTestShader.setVec3("lightPos", lightPos);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::println("START renderloop ******************************");

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

        /////////////////////////////////////
        ////// XYZ //////////////////////////
        /////////////////////////////////////

        xyzShader.useShader();
        xyzVao.bindVertexArray();
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(Data::xyz.size()));
   
        /////////////////////////////////////
        ////// Lights ///////////////////////
        /////////////////////////////////////

        multiLight.useShader();
        // Directional Light
        glm::vec3 dirLightDirection{ 0.3f, 1.0f, 1.0f };
        dirLightDirection = view * glm::vec4(dirLightDirection, 0.0f);
        multiLight.setVec3("dirLight.direction", dirLightDirection);
        // Spotlight direction
        glm::vec3 spotLightLightDirection{ 0.0f, -1.0f, 0.0f };
        spotLightLightDirection = view * glm::vec4(spotLightLightDirection, 0.0f);
        multiLight.setVec3("spotLight.direction", spotLightLightDirection);
        // Spotlight position
        lightPos = glm::vec3((3.0f * sin(glfwGetTime())), 6.5f, (4.5f * cos(glfwGetTime())));
        lightPos = view * glm::vec4(lightPos, 1.0f);
        multiLight.setVec3("spotLight.position", lightPos);
        // Spotlight color
        spotLightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0f)) + 0.2f;
        spotLightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7f)) + 0.2f;
        spotLightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3f)) + 0.2f;
        multiLight.setVec3("spotLight.diffuse", (spotLightColor * glm::vec3(0.5f)));

        // Calculate Pointlight position in View Space
        std::vector<glm::vec3> pointLightPositionsViewSpace{};
        for (int i = 0; i < std::size(pointLightPositions); i++) {
            pointLightPositionsViewSpace.emplace_back(glm::vec3(view * glm::vec4(pointLightPositions[i], 1.0)));
        }

        multiLight.setVec3("pointLights[0].position", pointLightPositionsViewSpace[0]);  // red
        multiLight.setVec3("pointLights[1].position", pointLightPositionsViewSpace[1]);  // green
        multiLight.setVec3("pointLights[2].position", pointLightPositionsViewSpace[2]);  // blue
        multiLight.setVec3("pointLights[3].position", pointLightPositionsViewSpace[3]);  // white

        /////////////////////////////////////
        glm::mat4 model{};
        /////////////////////////////////////

        /////////////////////////////////////
        ////// LightCube ////////////////////
        /////////////////////////////////////

        // spotlight - Draaiende LightCube
        lightShader.useShader();
        lightVao.bindVertexArray();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos); // View space
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        lightShader.setMat4("model", model);
        lightShader.setVec3("lightColor", spotLightColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // pointlights - 4 vaste LightCubes
        for (int i = 0; i < std::size(pointLightPositions); i++) {
            lightShader.useShader();
            lightVao.bindVertexArray();
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositionsViewSpace[i]);
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            lightShader.setMat4("model", model); // View space
            lightShader.setVec3("lightColor", pointLightColors[i]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        /////////////////////////////////////
        ////// Cubes ////////////////////////
        /////////////////////////////////////

        multiLight.useShader();
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

            // Set Normal in view space
            multiLight.setMat3("NormalViewCPU", glm::transpose(glm::inverse(view * model)));
            //multiLight.setMat3("NormalWorldCPU", glm::transpose(glm::inverse(model)));

            //glDrawArrays(GL_TRIANGLES, 0, 36);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Data::cube.size()), GL_UNSIGNED_INT, 0);
        }

        // render the loaded model
        ourModelShader.useShader(); // TODO eigen shader maken voor model?!
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.0f, 0.0f, 2.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourModelShader.setMat4("model", model);
        ourModel.Draw(ourModelShader); // Model::draw!

        //// cubeTest
        //cubeTestShader.useShader();
        //glm::mat4 model = glm::mat4(1.0f);
        ////cubeTestShader.setMat4("model", model);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, lightPos);
        //model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        //cubeTestShader.setMat4("model", model);
        //cubeTest.bindVertexArray();
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Data::cube.size()), GL_UNSIGNED_INT, 0);
        

        if (!Global::paused) {
            glfwSwapBuffers(window);
        }

        // TODO use correct Shader for flashlight input
        multiLight.useShader();
        glfwPollEvents();
    }
    Global::glCheckError();
    glfwTerminate();
    Global::glClearError(); // TODO glfwTerminate() produce errors
    Global::glCheckError();
    return 0;
}