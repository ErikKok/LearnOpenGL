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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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

    Global::initStencilBuffer();

    Global::glCheckError();

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
    cheap = sizeof(Texture) <= 2 * sizeof(void*);
    std::println("Is Texture cheap to copy: {} (provided there are no additional setup costs)", cheap);

    UniformBuffer projectionView(2 * sizeof(glm::mat4), 0);

    /////////////////////////////////////
    ////// Skybox ///////////////////////
    std::println("CREATE Skybox");///////

    VertexArray skyboxVao;
    VertexBuffer skyboxVbo(sizeof(Data::skybox1), &Data::skybox1);
    VertexAttributeLayout skyboxLayout;
    skyboxLayout.pushVertexAttributeLayout<float>(3);
    skyboxVao.addVertexAttributeLayout(skyboxVbo, skyboxLayout);

    Shader skyboxShader("Shaders\\skybox.shader");

    Texture cubemapTexture(Data::skybox1Faces);

    /////////////////////////////////////
    ////// XYZ //////////////////////////
    std::println("CREATE XYZ");//////////

    VertexArray xyzVao;
    VertexBuffer xyzVbo(sizeof(Data::xyz), &Data::xyz);
    VertexAttributeLayout xyzLayout;
    xyzLayout.pushVertexAttributeLayout<float>(3);
    xyzLayout.pushVertexAttributeLayout<float>(3);
    xyzVao.addVertexAttributeLayout(xyzVbo, xyzLayout);

    Shader xyzShader("Shaders\\xyz.shader");

    /////////////////////////////////////
    ////// Lights ///////////////////////
    std::println("CREATE Lights");///////

    Shader multiLight("Shaders\\multiLight.shader");

    multiLight.useShader();
    // Material
    multiLight.setInt("material.diffuse1", 8);
    multiLight.setInt("material.specular1", 9);
    multiLight.setInt("material.emission", 10);
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
    float spotLightOuterCutOff{ glm::cos(glm::radians(48.5f)) };
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
    std::println("CREATE LightCube");////

    VertexArray lightVao;
    VertexBuffer cubeVbo(sizeof(Data::cube), &Data::cube);
    VertexAttributeLayout lightLayout;
    lightLayout.pushVertexAttributeLayout<float>(3);
    lightLayout.setVertexStride(32);
    lightVao.addVertexAttributeLayout(cubeVbo, lightLayout);

    Shader lightShader("Shaders\\light.shader"); //lightCubeShader
    lightShader.useShader();

    /////////////////////////////////////
    ////// Cubes ////////////////////////
    std::println("CREATE Cubes");////////

    VertexArray cubeVao;
    VertexAttributeLayout cubeLayout;
    cubeLayout.pushVertexAttributeLayout<float>(3);
    cubeLayout.pushVertexAttributeLayout<float>(2);
    cubeLayout.pushVertexAttributeLayout<float>(3);
    cubeVao.addVertexAttributeLayout(cubeVbo, cubeLayout);

    ElementBuffer cubeTestEBO(sizeof(Data::cubeIndices), &Data::cubeIndices);

    Texture cubeDiffuse("Textures\\container2.png");
    Texture cubeSpecular("Textures\\container2_specular.png");
    Texture cubeEmission("Textures\\matrix.jpg");

    /////////////////////////////////////
    ////// Floor ////////////////////////
    std::println("CREATE Floor");////////

    VertexArray floorVao;
    VertexBuffer floorVbo(sizeof(Data::floor2), &Data::floor2);
    VertexAttributeLayout floorlayout{};
    floorlayout.pushVertexAttributeLayout<float>(3);
    floorlayout.pushVertexAttributeLayout<float>(2);
    floorlayout.pushVertexAttributeLayout<float>(3);
    floorVao.addVertexAttributeLayout(floorVbo, floorlayout);
    ElementBuffer floorEbo(sizeof(Data::floorIndices), &Data::floorIndices);

    Texture floor("Textures\\floor.jpg");

    //////// Outline ////////////////////

    Shader singleColor("Shaders\\singleColor.shader");

    ////////////////////////////////////
    ////// Mesh ////////////////////////
    std::println("LOAD Model");/////////

    float loadTime{ static_cast<float>(glfwGetTime()) };


    Shader ourModelShader("Shaders\\multiLight.shader");
    Model ourModel("Models/Backpack/backpack.obj");
    //Model ourModel("Models/Nanosuit/nanosuit.obj");
    //Model ourModel("Models/Cyborg/cyborg.obj");
    //Model ourModel("Models/Mars/planet.obj");
    //Model ourModel("Models/Rock/rock.obj");
    //Model ourModel("Models/Vampire/dancing_vampire.dae"); // crash
    //Model ourModel("FinalBaseMesh.obj"); // TODO laadt niet 100%

    //Global::deltaTime = currentFrame - Global::lastFrame;
    //Global::lastFrame = currentFrame;
    std::println("Load time model: {} seconds", static_cast<float>(glfwGetTime()) - loadTime );
    
    ////////////////////////////////////////////////

    // Visualize normals
    Shader normal("Shaders\\normal.shader");

    ////////////////////////////////////////////////
    // All textures get bind here, set sampler2D uniforms to the correct texture unit before draw call
    std::println("AI TEXTURE ASSET MANAGER ***********************"); 
    Texture black(0x00000000);          //
    black.bindTexture(0);               // 0
    Texture white(0xffffffff);          //
    white.bindTexture(1);               // 1
    // Unused                           // 2, 3
    floor.bindTexture(4);               // 4
    // Unused                           // 3 - 7
    cubeDiffuse.bindTexture(8);         // 8
    cubeSpecular.bindTexture(9);        // 9
    cubeEmission.bindTexture(10);       // 10
    // Unused                           // 11 - 15
    // Reserved for Model::Draw         // 16 - 31
    ////////////////////////////////////////////////

    float outlineAlpha{ 0.0f };
    Global::getInformation();
    Global::glCheckError();
    std::println("START Renderloop *******************************");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
        Global::clearStencilBuffer();

        // per-frame time logic
        float currentFrame{ static_cast<float>(glfwGetTime()) };
        Global::deltaTime = currentFrame - Global::lastFrame;
        Global::lastFrame = currentFrame;
        //std::println("deltaTime: {}ms", Global::deltaTime * 1000);
        //std::println("Position: {}, {}, {}", Global::camera.m_position.x, Global::camera.m_position.y, Global::camera.m_position.z);
        //std::println("Front: {}, {}, {}", Global::camera.m_front.x, Global::camera.m_front.y, Global::camera.m_front.z);

        Global::processInput(window);

        //Global::camera.fakeGravity(Global::deltaTime);
        
        // Set uniformBuffer
        /////////////////////////////////////
        //projectionView.bindUniformBuffer();
        BufferSubDataLayout projectionViewLayout{};
        projectionViewLayout.pushUniformBufferSubData(Global::camera.getProjectionMatrix());
        glm::mat4 view{ Global::camera.GetViewMatrix() };
        projectionViewLayout.pushUniformBufferSubData(view);
        projectionView.addUniformBufferSubData(projectionViewLayout);

        // Init uniforms
        /////////////////////////////////////
        glm::mat4 model{};
        glm::mat4 modelView{};
        
        /////////////////////////////////////
        ////// XYZ //////////////////////////
        /////////////////////////////////////

        xyzShader.useShader();
        xyzVao.bindVertexArray();
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(Data::xyz.size()));
        //std::println("XYZ ##################");
        
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
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 1);

        // pointlights - 4 vaste LightCubes
        for (int i = 0; i < std::size(pointLightPositions); i++) {
            lightShader.useShader();
            lightVao.bindVertexArray();
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositionsViewSpace[i]);
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            lightShader.setMat4("model", model); // View space
            lightShader.setVec3("lightColor", pointLightColors[i]);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 1);
        }

        /////////////////////////////////////
        ////// Cubes ////////////////////////
        /////////////////////////////////////
    
        multiLight.useShader();
        multiLight.setInt("material.diffuse1", 8);
        multiLight.setInt("material.specular1", 9);
        multiLight.setInt("material.emission", 10);
        cubeVao.bindVertexArray();

        //struct Instance {
        //    glm::mat4 modelView{};
        //    glm::mat4 NormalViewCPU{};
        //};
        //Instance instance{};
        //std::vector<Instance> instancedVector{};

        /////////////////////////////////////////////

        std::vector<glm::mat4> ssbo1ModelViewVector{};
        std::vector<glm::mat4> ssbo2NormalViewCPUVector{};

        for (unsigned int i = 0u; i < 10; i++)
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
            modelView = view * model;

            ssbo1ModelViewVector.emplace_back(modelView);
            ssbo2NormalViewCPUVector.emplace_back(glm::transpose(glm::inverse(modelView)));
            
            //instance.modelView = modelView;
            //instance.NormalViewCPU = glm::transpose(glm::inverse(modelView));
            //instancedVector.emplace_back(instance);
        }

        GLuint ssboModelView{};
        glCreateBuffers(1, &ssboModelView);
        glNamedBufferStorage(ssboModelView, sizeof(glm::mat4) * ssbo1ModelViewVector.size(), (const void*)ssbo1ModelViewVector.data(), GL_DYNAMIC_STORAGE_BIT);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboModelView);

        GLuint ssboNormalViewCPU{};
        glCreateBuffers(1, &ssboNormalViewCPU);
        glNamedBufferStorage(ssboNormalViewCPU, sizeof(glm::mat4) * ssbo2NormalViewCPUVector.size(), (const void*)ssbo2NormalViewCPUVector.data(), GL_DYNAMIC_STORAGE_BIT);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssboNormalViewCPU);

        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::cube.size()), GL_UNSIGNED_INT, 0, 2 * static_cast<GLsizei>(ssbo1ModelViewVector.size()));

        //GLuint instancessbo{};
        //glCreateBuffers(1, &instancessbo);
        //glNamedBufferStorage(instancessbo, (2 * sizeof(glm::mat4)) * instancedVector.size(), (const void*)instancedVector.data(), GL_DYNAMIC_STORAGE_BIT);
        //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, instancessbo);

        //glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::cube.size()), GL_UNSIGNED_INT, 0, 2 * static_cast<GLsizei>(instancedVector.size())); // 2 * !!??

        //////////////////////////////
        // Not instanced: old:

        //multiLight.useShader();
        //multiLight.setInt("material.diffuse1", 8);
        //multiLight.setInt("material.specular1", 9);
        //multiLight.setInt("material.emission", 10);
        //cubeVao.bindVertexArray();
        //for (unsigned int i = 0; i < 10; i++)
        //{
        //    model = glm::mat4(1.0f);
        //    model = glm::translate(model, Data::cubePositions[i]);
        //    if (i == 2 || i == 5 || i == 8) {
        //        float angle = 25.0f + (15 * i);
        //        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(100.0f) * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
        //    }
        //    if (i == 3) {
        //        model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -3.0f));
        //        model = glm::scale(model, glm::vec3(20.0, 20.0, 1.0));
        //    }
        //    //if (i == 9) {
        //    //    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        //    //    model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
        //    //}
        //    //multiLight.setMat4("model", model);
        //    modelView = view * model;
        //    multiLight.setMat4("modelView", modelView);
        //    // Set Normal in view space
        //    multiLight.setMat3("NormalViewCPU", glm::transpose(glm::inverse(modelView)));
        //    //glDrawArrays(GL_TRIANGLES, 0, 36);
        //    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::cube.size()), GL_UNSIGNED_INT, 0, 1);

        //    // Visualize normals
        //    normal.useShader();
        //    normal.setMat4("modelView", modelView);
        //    normal.setMat3("NormalViewCPU", glm::transpose(glm::inverse(modelView)));
        //    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::cube.size()), GL_UNSIGNED_INT, 0, 1);

        //    multiLight.useShader();
        //}

        //std::println("##################");

        /////////////////////////////////////
        ////// Model ////////////////////////
        /////////////////////////////////////

        //ourModelShader.useShader(); // TODO eigen shader maken voor Model?! Nee toch?
        multiLight.useShader();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.0f, 3.0f, 2.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        modelView = view * model;
        
        ssbo1ModelViewVector[0] = modelView;
        glNamedBufferSubData(ssboModelView, 0, sizeof(glm::mat4) * ssbo1ModelViewVector.size(), (const void*)ssbo1ModelViewVector.data());

        ssbo2NormalViewCPUVector[0] = glm::transpose(glm::inverse(modelView));
        glNamedBufferSubData(ssboNormalViewCPU, 0, sizeof(glm::mat4) * ssbo2NormalViewCPUVector.size(), (const void*)ssbo2NormalViewCPUVector.data());

        multiLight.setInt("material.emission", 0); // black

        ourModel.Draw(multiLight);

        /////////////////////////////////////
        ////// Floor ////////////////////////
        /////////////////////////////////////

        glStencilMask(0xFF); // enable writing to the stencil buffer
        multiLight.useShader();
        floorVao.bindVertexArray();
        multiLight.setInt("material.diffuse1", 4);
        multiLight.setInt("material.specular1", 0); // black
        multiLight.setInt("material.emission", 0); // black

        //Global::transformNormalViewCPU(multiLight, glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0, 0.0, 0.0), glm::vec3(25.0, 25.0, 2.0), view); //////////////////////////////////////
        
        ssbo1ModelViewVector[0] = Global::getModelView(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0, 0.0, 0.0), glm::vec3(25.0, 25.0, 2.0), view);
        glNamedBufferSubData(ssboModelView, 0, sizeof(glm::mat4)* ssbo1ModelViewVector.size(), (const void*)ssbo1ModelViewVector.data());

        ssbo2NormalViewCPUVector[0] = glm::transpose(glm::inverse(ssbo1ModelViewVector[0]));
        glNamedBufferSubData(ssboNormalViewCPU, 0, sizeof(glm::mat4)* ssbo2NormalViewCPUVector.size(), (const void*)ssbo2NormalViewCPUVector.data());

        glDisable(GL_CULL_FACE); // disable because floor has no Z dimension, the same face is the front AND back, I think...
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::floor2.size()), GL_UNSIGNED_INT, 0, 1);

        glStencilMask(0x00); // disable writing to the stencil buffer

        /////////////////////////////////////
        ////// Skybox ///////////////////////
        /////////////////////////////////////

        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.useShader();
        skyboxShader.setMat4("view", glm::mat3(Global::camera.GetViewMatrix())); // remove translation from the view matrix

        // skybox cube
        skyboxVao.bindVertexArray();
        //cubemapTexture.bindTexture();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS); // set depth function back to default

        /////////////////////////////////////
        ////// Floor Outline ////////////////
        /////////////////////////////////////
   
        // Until order independent transparency is implemented, partly transparant objects need to be drawn last (even after the skybox)
        if (Global::drawOutline) {  
            singleColor.useShader();

            if (outlineAlpha >= 0.0f)
                outlineAlpha += 0.01f;
            if (outlineAlpha >= 1.0f)
                outlineAlpha = 0.0f;
            glm::vec4 color{ 1.0f, 0.28f, 0.26f, 0.0f };
            //color.x = static_cast<float>(sin(glfwGetTime() * 2.0f)) + 0.2f;
            //color.y = static_cast<float>(sin(glfwGetTime() * 0.7f)) + 0.2f;
            //color.z = static_cast<float>(sin(glfwGetTime() * 1.3f)) + 0.2f;
            color.w = outlineAlpha;
            singleColor.setVec4("color", color); // (color * glm::vec4(0.5f)));

            glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // only draw according to stencil buffer
            //glDisable(GL_DEPTH_TEST); // disable depth testing makes following draw calls drawn on top of the outline
            // Scale Floor
            
            ssbo1ModelViewVector[0] = Global::getModelView(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0, 0.0, 0.0), glm::vec3(26.0, 26.0, 2.0), view);
            //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboModelView);
            glNamedBufferSubData(ssboModelView, 0, sizeof(glm::mat4) * ssbo1ModelViewVector.size(), (const void*)ssbo1ModelViewVector.data());
            floorVao.bindVertexArray();
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::floor2.size()), GL_UNSIGNED_INT, 0, 1);
            //glEnable(GL_DEPTH_TEST);
            
            // De-init Stencil Buffer
            glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test again
        }
        glEnable(GL_CULL_FACE);

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