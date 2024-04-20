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

    UniformBuffer projectionUbo(sizeof(glm::mat4), 0);

    /////////////////////////////////////
    ////// Skybox ///////////////////////
    std::println("CREATE Skybox");///////

    VertexArray skyboxVao;
    VertexBuffer skyboxVbo(sizeof(Data::skybox1), &Data::skybox1);
    VertexAttributeLayout skyboxLayout;
    skyboxLayout.pushVertexAttributeLayout<float>(3);
    skyboxVao.addVertexAttributeLayout(skyboxVbo, skyboxLayout);
    Shader skyboxShader("Shaders\\skybox.shader");

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

    // DirLight
    glm::vec3 dirLightDirection{ 0.7f, 0.9f, 0.8f }; // World space
    glm::vec3 dirLightColor(1.0f, 1.0f, 0.95f);
    multiLight.setVec3("dirLight.diffuse", (dirLightColor));
    multiLight.setFloat("dirLight.strength", 0.8f);
    multiLight.setFloat("dirLight.ambient", 0.3f);

    // PointLight (max 8, hard coded in shader)
    glm::vec3 pointLightPositions[] = { // World space
        glm::vec3(0.7f, 11.2f,   2.0f),
        glm::vec3(4.0f,  2.0f, -12.0f),
        glm::vec3(-4.0f,  2.0f,  12.0f),
        glm::vec3(15.0f,  1.2f,  -3.0f),
    };

    glm::vec3 pointLightColors[] = {
    glm::vec3(1.0f, 1.0f, 1.0f), // red
    glm::vec3(0.0f, 1.0f, 0.0f), // green
    glm::vec3(0.0f, 0.0f, 1.0f), // blue
    glm::vec3(1.0f, 0.0f, 0.0f), // white
    };

    // TODO use variable length arrays through SSBO
    // Removes the need to manually set the pointLightsCount here AND in the shader
    // See -> https://computergraphics.stackexchange.com/questions/5323/dynamic-array-in-glsl
    multiLight.setInt("pointLightsCount", 4);

    multiLight.setVec3("pointLights[0].diffuse", pointLightColors[0]);  // red, distance 50
    multiLight.setFloat("pointLights[0].constant", 1.0f);
    multiLight.setFloat("pointLights[0].linear", 0.09f);
    multiLight.setFloat("pointLights[0].quadratic", 0.032f);
    multiLight.setFloat("pointLights[0].strength", 2.0f);

    multiLight.setVec3("pointLights[1].diffuse", pointLightColors[1]);  // green, distance 50
    multiLight.setFloat("pointLights[1].constant", 1.0f);
    multiLight.setFloat("pointLights[1].linear", 0.09f);
    multiLight.setFloat("pointLights[1].quadratic", 0.032f);
    multiLight.setFloat("pointLights[1].strength", 1.0f);

    multiLight.setVec3("pointLights[2].diffuse", pointLightColors[2]);  // blue, distance 325
    multiLight.setFloat("pointLights[2].constant", 1.0f);
    multiLight.setFloat("pointLights[2].linear", 0.09f);
    multiLight.setFloat("pointLights[2].quadratic", 0.032f);
    multiLight.setFloat("pointLights[2].strength", 1.0f);

    multiLight.setVec3("pointLights[3].diffuse", pointLightColors[3]);  // white, distance 13
    multiLight.setFloat("pointLights[3].constant", 1.0f);
    multiLight.setFloat("pointLights[3].linear", 0.35f);
    multiLight.setFloat("pointLights[3].quadratic", 0.44f);
    multiLight.setFloat("pointLights[3].strength", 3.0f);

    // SpotLight
    glm::vec3 spotLightLightDirection{ 0.0f, -1.0f, 0.0f };                 // World space
    float spotLightCutOff{ glm::cos(glm::radians(36.0f)) };                 // Inner cone
    float spotLightOuterCutOff{ glm::cos(glm::radians(48.0f)) };            // Outer cone
    multiLight.setFloat("spotLight.outerCutOff", spotLightOuterCutOff);
    float spotLightEpsilon{ spotLightCutOff - spotLightOuterCutOff };       // Gradually fade the light between inner and outer cone
    multiLight.setFloat("spotLight.epsilon", spotLightEpsilon);
    glm::vec3 spotLightColor{ 1.0f, 1.0f, 1.0f };
    multiLight.setVec3("spotLight.diffuse", spotLightColor);
    //multiLight.setFloat("spotLight.emission", 0.0f);
    multiLight.setFloat("spotLight.constant", 1.0f);
    multiLight.setFloat("spotLight.linear", 0.045f);
    multiLight.setFloat("spotLight.quadratic", 0.0075f);
    multiLight.setFloat("spotLight.strength", 2.0f);

    // FlashLight
    flashLight = &multiLight; // TODO
    multiLight.setBool("flashLight.on", false);
    float flashLightCutOff{ glm::cos(glm::radians(8.5f)) };                 // Inner cone
    float flashLightOuterCutOff{ glm::cos(glm::radians(12.5f)) };           // Outer cone
    multiLight.setFloat("flashLight.outerCutOff", flashLightOuterCutOff);
    float flashLightEpsilon{ flashLightCutOff - flashLightOuterCutOff };    // Gradually fade the light between inner and outer cone
    multiLight.setFloat("flashLight.epsilon", flashLightEpsilon);
    glm::vec3 flashlightColor{ 1.0f, 1.0f, 1.0f };
    multiLight.setVec3("flashLight.diffuse", flashlightColor);
    multiLight.setFloat("flashLight.constant", 1.0f);
    multiLight.setFloat("flashLight.linear", 0.045f);
    multiLight.setFloat("flashLight.quadratic", 0.0075f);
    multiLight.setFloat("flashLight.strength", 0.7f);
    multiLight.setVec3("flashLight.origin", 0.0f, 0.0f, 0.0f);
    multiLight.setFloat("flashLight.emission", 0.8f);

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
    std::println("Load time model: {} seconds", static_cast<float>(glfwGetTime()) - loadTime);

    ////////////////////////////////////////////////

    // Visualize normals
    Shader normal("Shaders\\normal.shader");

    /////////////////////////////////////////////////////////////////////////////////////
    // All textures get loaded and bind here
    // Set material uniforms to the correct texture unit / values before each draw call
    std::println("AI TEXTURE ASSET MANAGER ***********************");
    /* 01 GL_TEXTURE_CUBE_MAP */ Texture cubemapTexture(Data::skybox1Faces);
    /* 01 */ Texture black(0x00000000);                                         
    /* 01 */ Texture white(0xffffffff);                                        
    /* 02 */                        
    /* 03 */ Texture flashlight("Textures\\flashlight.jpg");
    /* 04 */ Texture floor("Textures\\floor.jpg");                              
    /* 05 */
    /* 06 */
    /* 07 */
    /* 08 */ Texture cubeDiffuse("Textures\\container2.png");                   
    /* 09 */ Texture cubeSpecular("Textures\\container2_specular.png");         
    /* 10 */ Texture cubeEmission("Textures\\emission.png");
    /* 11 */ Texture flashlightMap("Textures\\container2_emission.png");
    /* 12 */ Texture flashlightResult("Textures\\matrix.jpg");
    /* 13 */
    /* 14 */
    /* 15 */
    /* 16 - 31 */// Reserved for Model::Draw
    /////////////////////////////////////////////////////////////////////////////////////
    /* 01 */ black.bindTexture(0);
    /* 01 */ white.bindTexture(1);
    /* 02 */ 
    /* 03 */ flashlight.bindTexture(3);
    /* 04 */ floor.bindTexture(4);
    /* 05 */
    /* 06 */
    /* 07 */
    /* 08 */ cubeDiffuse.bindTexture(8);
    /* 09 */ cubeSpecular.bindTexture(9);
    /* 10 */ cubeEmission.bindTexture(10);
    /* 11 */ flashlightMap.bindTexture(11);
    /* 12 */ flashlightResult.bindTexture(12);
    /* 13 */
    /* 14 */
    /* 15 */
    /* 16 - 31 */// Reserved for Model::Draw
    /////////////////////////////////////////////////////////////////////////////////////

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

        Global::view = Global::camera.GetViewMatrix();
        Global::projection = Global::camera.getProjectionMatrix();

        // Init uniforms / ssbo's / uniform buffers
        /////////////////////////////////////
        glm::mat4 model{};
        glm::mat4 modelViewMatrix{};
        GLuint ssboModelViewMatrix{};
        glCreateBuffers(1, &ssboModelViewMatrix);
        std::vector<glm::mat4> ssboModelViewMatrixVector{};
        GLuint ssboNormalMatrixCPU{};
        glCreateBuffers(1, &ssboNormalMatrixCPU);
        std::vector<glm::mat4> ssboNormalMatrixVector{};
        GLuint ssboMVPMatrix{};
        glCreateBuffers(1, &ssboMVPMatrix);
        std::vector<glm::mat4> ssboMVPMatrixVector{};
        //projectionUbo.bindUniformBuffer();
        BufferSubDataLayout projectionLayout{}; // TODO rename projection
        projectionLayout.pushUniformBufferSubData(Global::projection);
        projectionUbo.addUniformBufferSubData(projectionLayout);

        /////////////////////////////////////
        ////// XYZ //////////////////////////
        /////////////////////////////////////

        xyzShader.useShader();
        xyzVao.bindVertexArray();
        xyzShader.setMat4("view", Global::view);
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(Data::xyz.size()));
        //std::println("XYZ ##################");

        /////////////////////////////////////
        ////// Lights ///////////////////////
        /////////////////////////////////////

        multiLight.useShader();
        // Transform Directional Light to View Space
        multiLight.setVec3("dirLight.direction", Global::view * glm::vec4(dirLightDirection, 0.0f));            // View space
        // Transform Pointlight positions to View Space
        multiLight.setVec3("pointLights[0].position", glm::vec3(Global::view * glm::vec4(pointLightPositions[0], 1.0)));  // red
        multiLight.setVec3("pointLights[1].position", glm::vec3(Global::view * glm::vec4(pointLightPositions[1], 1.0)));  // green
        multiLight.setVec3("pointLights[2].position", glm::vec3(Global::view * glm::vec4(pointLightPositions[2], 1.0)));  // blue
        multiLight.setVec3("pointLights[3].position", glm::vec3(Global::view * glm::vec4(pointLightPositions[3], 1.0)));  // white
        // Transform Spotlight direction to View Space
        multiLight.setVec3("spotLight.direction", Global::view * glm::vec4(spotLightLightDirection, 0.0f));     // View space
        // Calculate Spotlight position and transform to View Space
        //lightPos(0.0f, 5.2f, 3.0f);
        glm::vec3 lightPos = glm::vec3((3.0f * sin(glfwGetTime())), 6.5f, (4.5f * cos(glfwGetTime())));         // World space
        multiLight.setVec3("spotLight.position", Global::view * glm::vec4(lightPos, 1.0f));                     // View space
        // Calculate Spotlight color
        spotLightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0f)) + 0.2f;
        spotLightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7f)) + 0.2f;
        spotLightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3f)) + 0.2f;
        multiLight.setVec3("spotLight.diffuse", (spotLightColor * glm::vec3(0.5f))); // TODO remove float, is same as Strength?

        /////////////////////////////////////
        ////// LightCube ////////////////////
        /////////////////////////////////////

        // spotlight - Draaiende LightCube
        lightShader.useShader();
        lightVao.bindVertexArray();
        lightShader.setMat4("MVPMatrix", Global::projection * Global::getModelViewMatrix(glm::vec3(lightPos), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)));
        lightShader.setVec3("lightColor", spotLightColor);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 1);

        // pointlights - 4 vaste LightCubes
        for (int i = 0; i < std::size(pointLightPositions); i++) {
            lightShader.useShader();
            lightVao.bindVertexArray();
            lightShader.setMat4("MVPMatrix", Global::projection * Global::getModelViewMatrix(glm::vec3(pointLightPositions[i]), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)));
            lightShader.setVec3("lightColor", pointLightColors[i]);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 1);
        }

        /////////////////////////////////////
        ////// Cubes ////////////////////////
        /////////////////////////////////////

        multiLight.useShader();
        multiLight.setFloat("material.shininess", 8.0f);
        multiLight.setInt("material.diffuse1", 8);
        multiLight.setInt("material.specular1", 9);
        multiLight.setInt("material.emission", 10);
        multiLight.setFloat("material.emissionStrength", 1.0f);
        multiLight.setInt("material.flashlightMap", 11);
        multiLight.setInt("material.flashlightResult", 12);
        multiLight.setFloat("flashLight.emission", 1.2f);

        cubeVao.bindVertexArray();

        for (unsigned int i = 0u; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, Data::cubePositions[i]);
            if (i == 2 || i == 5 || i == 8) {
                float angle = 25.0f + (15 * i);
                model = glm::rotate(model, (float)glfwGetTime() * glm::radians(100.0f) * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            }
            if (i == 3) { // wall
                model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -3.0f));
                model = glm::scale(model, glm::vec3(20.0, 20.0, 1.0));
            }
            //if (i == 9) { // floor
            //    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            //    model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
            //}

            modelViewMatrix = Global::view * model;
            ssboModelViewMatrixVector.emplace_back(modelViewMatrix);
            ssboNormalMatrixVector.emplace_back(glm::transpose(glm::inverse(modelViewMatrix)));
            ssboMVPMatrixVector.emplace_back(Global::projection * modelViewMatrix);
        }

        glNamedBufferStorage(ssboModelViewMatrix, sizeof(glm::mat4) * ssboModelViewMatrixVector.size(), (const void*)ssboModelViewMatrixVector.data(), GL_DYNAMIC_STORAGE_BIT);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboModelViewMatrix);

        glNamedBufferStorage(ssboNormalMatrixCPU, sizeof(glm::mat4) * ssboNormalMatrixVector.size(), (const void*)ssboNormalMatrixVector.data(), GL_DYNAMIC_STORAGE_BIT);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssboNormalMatrixCPU);

        glNamedBufferStorage(ssboMVPMatrix, sizeof(glm::mat4) * ssboMVPMatrixVector.size(), (const void*)ssboMVPMatrixVector.data(), GL_DYNAMIC_STORAGE_BIT);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssboMVPMatrix);

        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::cube.size()), GL_UNSIGNED_INT, 0, 2 * static_cast<GLsizei>(ssboModelViewMatrixVector.size()));

        /////////////////////////////////////
        ////// Model ////////////////////////
        /////////////////////////////////////

        multiLight.useShader();

        modelViewMatrix = Global::getModelViewMatrix(glm::vec3(4.0f, 3.0f, 2.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        ssboModelViewMatrixVector[0] = modelViewMatrix;
        glNamedBufferSubData(ssboModelViewMatrix, 0, sizeof(glm::mat4) * ssboModelViewMatrixVector.size(), (const void*)ssboModelViewMatrixVector.data());

        ssboNormalMatrixVector[0] = glm::transpose(glm::inverse(modelViewMatrix));
        glNamedBufferSubData(ssboNormalMatrixCPU, 0, sizeof(glm::mat4) * ssboNormalMatrixVector.size(), (const void*)ssboNormalMatrixVector.data());

        ssboMVPMatrixVector[0] = Global::projection * modelViewMatrix;
        glNamedBufferSubData(ssboMVPMatrix, 0, sizeof(glm::mat4) * ssboMVPMatrixVector.size(), (const void*)ssboMVPMatrixVector.data());

        multiLight.setFloat("material.shininess", 64.0f);
        multiLight.setInt("material.emission", 0); // black - 'disable' emission with a black texture
        multiLight.setFloat("flashLight.emission", 0.0f);
        ourModel.Draw(multiLight);

        /////////////////////////////////////
        ////// Floor ////////////////////////
        /////////////////////////////////////

        glStencilMask(0xFF); // enable writing to the stencil buffer
        glDisable(GL_CULL_FACE); // disable because floor has no Z dimension, the same face is the front AND back, I think...

        multiLight.useShader();
        floorVao.bindVertexArray();
        multiLight.setFloat("material.shininess", 1.0f);
        multiLight.setInt("material.diffuse1", 4);
        multiLight.setInt("material.specular1", 0); // black
        multiLight.setFloat("material.emissionStrength", 0.0f); // 'disable' emission with zero strength
        multiLight.setFloat("flashLight.emission", 0.0f);

        modelViewMatrix = Global::getModelViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(25.0f, 25.0f, 2.0f));
        ssboModelViewMatrixVector[0] = modelViewMatrix;
        glNamedBufferSubData(ssboModelViewMatrix, 0, sizeof(glm::mat4) * ssboModelViewMatrixVector.size(), (const void*)ssboModelViewMatrixVector.data());

        ssboNormalMatrixVector[0] = glm::transpose(glm::inverse(modelViewMatrix));
        glNamedBufferSubData(ssboNormalMatrixCPU, 0, sizeof(glm::mat4) * ssboNormalMatrixVector.size(), (const void*)ssboNormalMatrixVector.data());

        ssboMVPMatrixVector[0] = Global::projection * modelViewMatrix;
        glNamedBufferSubData(ssboMVPMatrix, 0, sizeof(glm::mat4) * ssboMVPMatrixVector.size(), (const void*)ssboMVPMatrixVector.data());

        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::floor2.size()), GL_UNSIGNED_INT, 0, 1);

        glStencilMask(0x00); // disable writing to the stencil buffer

        /////////////////////////////////////
        ////// Skybox ///////////////////////
        /////////////////////////////////////

        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.useShader();
        skyboxShader.setMat4("viewTranslationRemoved", glm::mat3(Global::view)); // Global::camera.GetViewMatrix())); // remove translation from the view matrix (cast to mat3 and back to mat4)

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
            modelViewMatrix = Global::getModelViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0, 0.0, 0.0), glm::vec3(26.0, 26.0, 2.0));
            ssboModelViewMatrixVector[0] = Global::projection * modelViewMatrix;
            //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboModelView);
            glNamedBufferSubData(ssboModelViewMatrix, 0, sizeof(glm::mat4) * ssboModelViewMatrixVector.size(), (const void*)ssboModelViewMatrixVector.data());
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