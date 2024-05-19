#pragma once

#include "Buffers.h"
#include "Camera.h"
#include "Data.h"
#include "Global.h"
#include "Light.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
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

    Global::view = Global::camera.getViewMatrix();

    /////////////////////////////////////
    ////// Skybox ///////////////////////
    std::println("CREATE Skybox");///////

    VertexArray skyboxVAO;
    VertexBuffer skyboxVBO(sizeof(Data::skybox1), &Data::skybox1);
    VertexAttributeLayout skyboxLayout;
    skyboxLayout.pushVertexAttributeLayout<float>(3);
    skyboxVAO.addVertexAttributeLayout(skyboxVBO, skyboxLayout);
    Shader skyboxShader("Shaders\\skybox.shader");

    /////////////////////////////////////
    ////// XYZ //////////////////////////
    std::println("CREATE XYZ");//////////

    VertexArray xyzVAO;
    VertexBuffer xyzVBO(sizeof(Data::xyz), &Data::xyz);
    VertexAttributeLayout xyzLayout;
    xyzLayout.pushVertexAttributeLayout<float>(3);
    xyzLayout.pushVertexAttributeLayout<float>(3);
    xyzVAO.addVertexAttributeLayout(xyzVBO, xyzLayout);
    Shader xyzShader("Shaders\\xyz.shader");

    /////////////////////////////////////
    ////// Lights ///////////////////////
    std::println("CREATE Lights");///////

    Shader multiLight("Shaders\\multiLight.shader");
    multiLight.useShader();



    // PointLight (max amount hard coded in shader TODO)
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

    multiLight.setVec3("pointLights[0].color", pointLightColors[0]);  // red, distance 50
    multiLight.setFloat("pointLights[0].constant", 1.0f);
    multiLight.setFloat("pointLights[0].linear", 0.09f);
    multiLight.setFloat("pointLights[0].quadratic", 0.032f);
    multiLight.setFloat("pointLights[0].strength", 1.0f);

    multiLight.setVec3("pointLights[1].color", pointLightColors[1]);  // green, distance 50
    multiLight.setFloat("pointLights[1].constant", 1.0f);
    multiLight.setFloat("pointLights[1].linear", 0.09f);
    multiLight.setFloat("pointLights[1].quadratic", 0.032f);
    multiLight.setFloat("pointLights[1].strength", 2.0f);

    multiLight.setVec3("pointLights[2].color", pointLightColors[2]);  // blue, distance 325
    multiLight.setFloat("pointLights[2].constant", 1.0f);
    multiLight.setFloat("pointLights[2].linear", 0.09f);
    multiLight.setFloat("pointLights[2].quadratic", 0.032f);
    multiLight.setFloat("pointLights[2].strength", 3.0f);

    multiLight.setVec3("pointLights[3].color", pointLightColors[3]);  // white, distance 13
    multiLight.setFloat("pointLights[3].constant", 1.0f);
    multiLight.setFloat("pointLights[3].linear", 0.35f);
    multiLight.setFloat("pointLights[3].quadratic", 0.44f);
    multiLight.setFloat("pointLights[3].strength", 1.0f);

    /////////////////////////////////////
    ////// light / depthMap /////////////
    std::println("CREATE Light / DepthMap");

    // DirLight
    DirectionalLight sun;
    sun.setDirection(0.7f, 0.9f, 0.8f);
    sun.setColor(1.0f, 1.0f, 0.95f);
    sun.setStrength(0.25f);
    sun.setDepthMap(2);
    sun.setAmbient(0.3f);
    sun.sendToShader(multiLight);

    // DirLight depthMap
    Texture depthMapDirLight(textureType::depthMap, 4096, 4096);
    FrameBuffer depthMapDirLightFBO(depthMapDirLight);
    depthMapDirLightFBO.setOrthographic(true);
    depthMapDirLightFBO.setNearPlane(10.0f);
    depthMapDirLightFBO.calculateProjectionMatrixOrthographic();
    depthMapDirLightFBO.setViewMatrix(glm::lookAt(sun.getDirection(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    depthMapDirLightFBO.calculateViewProjectionMatrix();
    Shader shadowMapDirLight("Shaders\\shadowMapDirLight.shader");

    // SpotLight
    SpotLight spotlight;
    spotlight.setPosition(0.0f, -1.0f, 0.0f);
    spotlight.setDirection(0.0f, -1.0f, 0.0f);
    spotlight.setColor(1.0f, 1.0f, 1.0f);
    spotlight.setStrength(1.2f);
    spotlight.setDepthMap(5);
    spotlight.setConstant(1.0f);
    spotlight.setLinear(0.014f);
    spotlight.setQuadratic(0.07f);
    spotlight.setInnerCutOff(36.0f);
    spotlight.setOuterCutOff(48.0f);
    spotlight.sendToShader(multiLight);

    // SpotLight depthMap
    Texture depthMapSpotLight(textureType::depthMap, 4096, 4096);
    FrameBuffer depthMapSpotLightFBO(depthMapSpotLight);
    depthMapSpotLightFBO.setFov(84.0f);
    depthMapSpotLightFBO.calculateProjectionMatrixPerspective(depthMapSpotLight);
    Shader shadowMapSpotLight("Shaders\\shadowMapSpotLight.shader");

    // FlashLight
    FlashLight flashLight;
    flashLight.setOn(false);
    flashLight.setPosition(0.0f, -1.0f, 0.0f);
    flashLight.setDirection(0.0f, -1.0f, 0.0f);
    flashLight.setColor(1.0f, 1.0f, 1.0f);
    flashLight.setStrength(2.5f);
    flashLight.setDepthMap(6);
    flashLight.setConstant(1.0f);
    flashLight.setLinear(0.045f);
    flashLight.setQuadratic(0.0075f);
    flashLight.setInnerCutOff(8.5f);
    flashLight.setOuterCutOff(12.5f);
    flashLight.setEmissionStrength(0.8f);
    flashLight.setOffset(0.4f, -0.5f, -0.3f);
    flashLight.sendToShader(multiLight);

    // FlashLight depthMap
    Texture depthMapFlashLight(textureType::depthMap, 4096, 4096);
    FrameBuffer depthMapFlashLightFBO(depthMapFlashLight);
    depthMapFlashLightFBO.setFov(90.0f);
    depthMapFlashLightFBO.calculateProjectionMatrixPerspective(depthMapFlashLight);
    Shader shadowMapFlashLight("Shaders\\shadowMapFlashLight.shader");   

    /////////////////////////////////////
    ////// LightCube ////////////////////
    std::println("CREATE LightCube");////

    VertexArray lightVAO;
    VertexBuffer cubeVBO(sizeof(Data::cube), &Data::cube);
    VertexAttributeLayout lightLayout;
    lightLayout.pushVertexAttributeLayout<float>(3);
    lightLayout.setVertexStride(32);
    lightVAO.addVertexAttributeLayout(cubeVBO, lightLayout);

    /////////////////////////////////////
    ////// Cubes ////////////////////////
    std::println("CREATE Cubes");////////

    VertexArray cubeVAO;
    VertexAttributeLayout cubeLayout;
    cubeLayout.pushVertexAttributeLayout<float>(3);
    cubeLayout.pushVertexAttributeLayout<float>(2);
    cubeLayout.pushVertexAttributeLayout<float>(3);
    cubeVAO.addVertexAttributeLayout(cubeVBO, cubeLayout);
    ElementBuffer cubeTestEBO(sizeof(Data::cubeIndices), &Data::cubeIndices);

    /////////////////////////////////////
    ////// Floor ////////////////////////
    std::println("CREATE Floor");////////

    VertexArray floorVAO;
    VertexBuffer floorVBO(sizeof(Data::floor2), &Data::floor2);
    VertexAttributeLayout floorLayout{};
    floorLayout.pushVertexAttributeLayout<float>(3);
    floorLayout.pushVertexAttributeLayout<float>(2);
    floorLayout.pushVertexAttributeLayout<float>(3);
    floorVAO.addVertexAttributeLayout(floorVBO, floorLayout);
    ElementBuffer floorEBO(sizeof(Data::floorIndices), &Data::floorIndices);
    float floorOutlineAlpha{ 0.0f };

    ////////////////////////////////////
    ////// Models //////////////////////
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

    /////////////////////////////////////////////////////////////////////////////////////
    // All textures get loaded and bind here
    // Set material uniforms to the correct texture unit / values before each draw call
    std::println("AI TEXTURE ASSET MANAGER ***********************");
    /* 01 GL_TEXTURE_CUBE_MAP */ Texture cubemapTexture(Data::skybox1Faces);
    /* 01 */ Texture blackTexture(0x00000000);                                         
    /* 01 */ Texture whiteTexture(0xffffffff);                                        
    /* 02 */                        
    /* 03 */ Texture flashlightTexture("Textures\\flashlight.jpg");
    /* 04 */ Texture floorTexture("Textures\\floor.jpg");                              
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
    /* 01 */ blackTexture.bind(0);
    /* 01 */ whiteTexture.bind(1);
    /* 02 */ depthMapDirLight.bind(2);
    /* 03 */ flashlightTexture.bind(3);
    /* 04 */ floorTexture.bind(4);
    /* 05 */ depthMapSpotLight.bind(5);
    /* 06 */ depthMapFlashLight.bind(6);
    /* 07 */
    /* 08 */ cubeDiffuse.bind(8);
    /* 09 */ cubeSpecular.bind(9);
    /* 10 */ //cubeEmission.bindTexture(10);
    /* 11 */ flashlightMap.bind(11);
    /* 12 */ flashlightResult.bind(12);
    /* 13 */
    /* 14 */
    /* 15 */
    /* 16 - 31 */// Reserved for Model::Draw
    std::println("************************************************");
    /////////////////////////////////////////////////////////////////////////////////////
    
    Shader singleColor("Shaders\\singleColor.shader");

    // Init uniforms/variables/SSBO /////
    /////////////////////////////////////
    glm::mat4 model{};
    glm::mat4 modelViewMatrix{};
    const int ArrayCountSSBO{ 10 };
    ShaderStorageBuffer NormalMatrixSSBO(2, ArrayCountSSBO);
    ShaderStorageBuffer ModelViewMatrixSSBO(3, ArrayCountSSBO);
    ShaderStorageBuffer MVPMatrixSSBO(4, ArrayCountSSBO);
    ShaderStorageBuffer dirLightMVPMatrixSSBO(5, ArrayCountSSBO);
    ShaderStorageBuffer spotLightMVPMatrixSSBO(6, ArrayCountSSBO);
    ShaderStorageBuffer flashLightMVPMatrixSSBO(7, ArrayCountSSBO);

    Global::getInformation();
    Global::glCheckError();
    std::println("START Renderloop *******************************");
    std::println("************************************************");

    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float currentFrame{ static_cast<float>(glfwGetTime()) };
        Global::deltaTime = currentFrame - Global::lastFrame;
        Global::lastFrame = currentFrame;
        //std::println("deltaTime: {}ms", Global::deltaTime * 1000);
        //std::println("Position: {}, {}, {}", Global::camera.m_position.x, Global::camera.m_position.y, Global::camera.m_position.z);
        //std::println("Front: {}, {}, {}", Global::camera.m_front.x, Global::camera.m_front.y, Global::camera.m_front.z);
        Global::clearStencilBuffer();
        Global::processInput(window);
        //glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // magenta

        /////////////////////////////////////
        ////// Lights ///////////////////////
        /////////////////////////////////////

        multiLight.useShader();
        multiLight.setVec3("pointLights[0].position", glm::vec3(Global::view * glm::vec4(pointLightPositions[0], 1.0)));  // red
        multiLight.setVec3("pointLights[1].position", glm::vec3(Global::view * glm::vec4(pointLightPositions[1], 1.0)));  // green
        multiLight.setVec3("pointLights[2].position", glm::vec3(Global::view * glm::vec4(pointLightPositions[2], 1.0)));  // blue
        multiLight.setVec3("pointLights[3].position", glm::vec3(Global::view * glm::vec4(pointLightPositions[3], 1.0)));  // white

        // Transform Spotlight direction to View Space
        spotlight.updateDirection(multiLight);
        // Calculate Spotlight position and transform to View Space
        spotlight.setPosition(3.0f * static_cast<float>(sin(glfwGetTime())), 6.5f, static_cast<float>(4.5f * cos(glfwGetTime())));
        spotlight.updatePosition(multiLight);
        // Calculate Spotlight color
        spotlight.setColor(static_cast<float>(sin(glfwGetTime() * 0.25f)), static_cast<float>(sin(glfwGetTime() * 0.50f)), static_cast<float>(sin(glfwGetTime() * 0.75f)));
        spotlight.updateColor(multiLight);

        /////////////////////////////////////////////////////////////////////////////////////
        // Start ShadowPass dirLight ////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        loadTime = static_cast<float>(glfwGetTime());

        depthMapDirLightFBO.startDepthMap(shadowMapDirLight);

        /////////////////////////////////////
        ////// Cubes ShadowPass dirLight ////
        /////////////////////////////////////

        cubeVAO.bindVertexArray();

        for (unsigned int i = 0u; i < std::size(Data::cubePositions); i++)
        {
            assert(std::size(Data::cubePositions) <= ArrayCountSSBO && "Loop will create more instances then ssbo can hold");

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
            //if (i == 9) { // floor // TODO only draws this 10th (9th) cube when uncommented?
            //    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            //    model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
            //}
            dirLightMVPMatrixSSBO.setVector(depthMapDirLightFBO.getViewProjectionMatrix() * model, i);
        }
        dirLightMVPMatrixSSBO.updateAndBind();

        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::cube.size()), GL_UNSIGNED_INT, 0, std::size(Data::cubePositions));

        /////////////////////////////////////
        ////// Model ShadowPass dirLight ////
        /////////////////////////////////////

        model = Global::getModelMatrix(glm::vec3(4.0f, 3.0f, 2.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        dirLightMVPMatrixSSBO.setVectorAndUpdateAndBind(depthMapDirLightFBO.getViewProjectionMatrix() * model);

        ourModel.Draw(shadowMapDirLight);

        /////////////////////////////////////
        ////// Floor ShadowPass dirLight ////
        /////////////////////////////////////

        floorVAO.bindVertexArray();
        model = Global::getModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(25.0f, 25.0f, 2.0f)); // TODO deze waarde wordt met de 2e renderpass ook gebruikt, herbruiken dus
        dirLightMVPMatrixSSBO.setVectorAndUpdateAndBind(depthMapDirLightFBO.getViewProjectionMatrix() * model);

        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::floor2.size()), GL_UNSIGNED_INT, 0, 1);

        /////////////////////////////////////////////////////////////////////////////////////
        // End ShadowPass dirLight //////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        depthMapDirLightFBO.stopDepthMap();
        //std::println("End ShadowPass dirLight time: {}ms", (static_cast<float>(glfwGetTime()) - loadTime) * 1000);

        /////////////////////////////////////////////////////////////////////////////////////
        // Start ShadowPass spotLight ///////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        loadTime = static_cast<float>(glfwGetTime());

        depthMapSpotLightFBO.startDepthMap(shadowMapSpotLight);
        glm::vec3 cameraPos = spotlight.getPosition();
        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(cameraPos.x, 0.0f, cameraPos.z), glm::vec3(0.0f, 0.0f, -1.0f));
        depthMapSpotLightFBO.setViewMatrix(view);

        //glm::vec3 cameraPos = glm::normalize(glm::cross(glm::vec3(cameraFront), glm::vec3(0.0f, 1.0f, 0.0f)));

        //depthMapSpotLightFBO.setViewMatrix(glm::lookAt(spotlight.getPosition(), (cameraPos + spotlight.getDirection()), glm::vec3(0.0f, 1.0f, 0.0f)));
        depthMapSpotLightFBO.calculateViewProjectionMatrix();

        /////////////////////////////////////
        ////// Cubes ShadowPass spotLight ///
        /////////////////////////////////////

        cubeVAO.bindVertexArray();

        for (unsigned int i = 0u; i < std::size(Data::cubePositions); i++)
        {
            assert(std::size(Data::cubePositions) <= ArrayCountSSBO && "Loop will create more instances then ssbo can hold");

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
            //if (i == 9) { // floor // TODO only draws this 10th (9th) cube when uncommented?
            //    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            //    model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
            //}
            spotLightMVPMatrixSSBO.setVector(depthMapSpotLightFBO.getViewProjectionMatrix() * model, i);
        }
        spotLightMVPMatrixSSBO.updateAndBind();

        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::cube.size()), GL_UNSIGNED_INT, 0, std::size(Data::cubePositions));

        /////////////////////////////////////
        ////// Model ShadowPass spotLight ///
        /////////////////////////////////////

        model = Global::getModelMatrix(glm::vec3(4.0f, 3.0f, 2.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        spotLightMVPMatrixSSBO.setVectorAndUpdateAndBind(depthMapSpotLightFBO.getViewProjectionMatrix() * model);

        ourModel.Draw(shadowMapSpotLight);

        /////////////////////////////////////
        ////// Floor ShadowPass spotLight ///
        /////////////////////////////////////

        floorVAO.bindVertexArray();
        model = Global::getModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(25.0f, 25.0f, 2.0f)); // TODO deze waarde wordt met de 2e renderpass ook gebruikt, herbruiken dus
        spotLightMVPMatrixSSBO.setVectorAndUpdateAndBind(depthMapSpotLightFBO.getViewProjectionMatrix() * model);

        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::floor2.size()), GL_UNSIGNED_INT, 0, 1);

        /////////////////////////////////////////////////////////////////////////////////////
        // End ShadowPass spotLight /////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        depthMapSpotLightFBO.stopDepthMap();
        //std::println("End ShadowPass spotLight time: {}ms", (static_cast<float>(glfwGetTime()) - loadTime) * 1000);

        /////////////////////////////////////////////////////////////////////////////////////
        // Start ShadowPass flashLight //////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        loadTime = static_cast<float>(glfwGetTime());

        if (!Global::flashLightOnUpdated) {
            multiLight.useShader();
            flashLight.toggle(multiLight);
            Global::flashLightOnUpdated = true;
        }

        depthMapFlashLightFBO.startDepthMap(shadowMapFlashLight);
        depthMapFlashLightFBO.setViewMatrix(Global::cameraFlashLight.getViewMatrix());
        depthMapFlashLightFBO.calculateViewProjectionMatrix();

        /////////////////////////////////////
        ////// Cubes ShadowPass flashLight //
        /////////////////////////////////////

        cubeVAO.bindVertexArray();

        for (unsigned int i = 0u; i < std::size(Data::cubePositions); i++)
        {
            assert(std::size(Data::cubePositions) <= ArrayCountSSBO && "Loop will create more instances then ssbo can hold");

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
            //if (i == 9) { // floor // TODO only draws this 10th (9th) cube when uncommented?
            //    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            //    model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
            //}
            flashLightMVPMatrixSSBO.setVector(depthMapFlashLightFBO.getViewProjectionMatrix() * model, i);
        }
        flashLightMVPMatrixSSBO.updateAndBind();

        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::cube.size()), GL_UNSIGNED_INT, 0, std::size(Data::cubePositions));

        /////////////////////////////////////
        ////// Model ShadowPass flashLight //
        /////////////////////////////////////

        model = Global::getModelMatrix(glm::vec3(4.0f, 3.0f, 2.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        flashLightMVPMatrixSSBO.setVectorAndUpdateAndBind(depthMapFlashLightFBO.getViewProjectionMatrix() * model);

        ourModel.Draw(shadowMapFlashLight);

        /////////////////////////////////////
        ////// Floor ShadowPass flashLight //
        /////////////////////////////////////

        floorVAO.bindVertexArray();
        model = Global::getModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(25.0f, 25.0f, 2.0f)); // TODO deze waarde wordt met de 2e renderpass ook gebruikt, herbruiken dus
        flashLightMVPMatrixSSBO.setVectorAndUpdateAndBind(depthMapFlashLightFBO.getViewProjectionMatrix() * model);

        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::floor2.size()), GL_UNSIGNED_INT, 0, 1);

        /////////////////////////////////////////////////////////////////////////////////////
        // End ShadowPass flashLight ////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        depthMapFlashLightFBO.stopDepthMap();
        //std::println("End ShadowPass spotLight time: {}ms", (static_cast<float>(glfwGetTime()) - loadTime) * 1000);

        /////////////////////////////////////////////////////////////////////////////////////
        // Render scene normal //////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        loadTime = static_cast<float>(glfwGetTime());
        glViewport(0, 0, Global::windowWidth, Global::windowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Global::camera.fakeGravity(Global::deltaTime);
        Global::view = Global::camera.getViewMatrix();
        Global::projection = Global::camera.getProjectionMatrix();

        /////////////////////////////////////
        ////// XYZ //////////////////////////
        /////////////////////////////////////

        xyzShader.useShader();
        xyzVAO.bindVertexArray();
        xyzShader.setMat4("viewProjectionMatrix", Global::projection * Global::view);
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(Data::xyz.size()));

        // hier stond voorheen de Lights sectie
        
        /////////////////////////////////////
        ////// LightCube ////////////////////
        /////////////////////////////////////

        // spotlight - Draaiende LightCube
        singleColor.useShader();
        lightVAO.bindVertexArray();

        MVPMatrixSSBO.setVectorAndUpdateAndBind(Global::projection * Global::getModelViewMatrix(spotlight.getPosition(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)));

        singleColor.setVec4("color", glm::vec4(spotlight.getColor(), 1.0f));
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 1);

        // pointlights - 4 vaste LightCubes
        for (int i = 0; i < std::size(pointLightPositions); i++) {
            assert(std::size(pointLightPositions) <= ArrayCountSSBO && "Loop will create more instances then ssbo can hold");
            
            singleColor.useShader();
            lightVAO.bindVertexArray();
            MVPMatrixSSBO.setVector(Global::projection * Global::getModelViewMatrix(glm::vec3(pointLightPositions[i]), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)), i);
            singleColor.setVec4("color", glm::vec4(pointLightColors[i], 1.0f)); // TODO deze uniform is geen array, dus kleur is zelfde voor alle cubes
        }
        MVPMatrixSSBO.updateAndBind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, std::size(pointLightPositions));

        /////////////////////////////////////
        ////// Cubes ////////////////////////
        /////////////////////////////////////

        multiLight.useShader();
        multiLight.setFloat("material.shininess", 32.0f);
        multiLight.setInt("material.diffuse1", 8);
        multiLight.setInt("material.specular1", 9);
        multiLight.setInt("material.emission", 10);
        multiLight.setFloat("material.emissionStrength", 0.5f);
        multiLight.setInt("material.flashlightMap", 11);
        multiLight.setInt("material.flashlightResult", 12);
        multiLight.setFloat("flashLight.emissionStrength", 1.0f);

        cubeVAO.bindVertexArray();

        for (unsigned int i = 0u; i < std::size(Data::cubePositions); i++)
        {
            assert(std::size(Data::cubePositions) <= ArrayCountSSBO && "Loop will create more instances then ssbo can hold");

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
            //if (i == 9) { // floor // TODO only draws this 10th (9th) cube when uncommented?
            //    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            //    model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
            //}

            modelViewMatrix = Global::view * model;
            NormalMatrixSSBO.setVector(glm::transpose(glm::inverse(modelViewMatrix)), i);
            ModelViewMatrixSSBO.setVector(modelViewMatrix, i);
            MVPMatrixSSBO.setVector(Global::projection * modelViewMatrix, i);
            dirLightMVPMatrixSSBO.setVector(depthMapDirLightFBO.getViewProjectionMatrix() * model, i);
            spotLightMVPMatrixSSBO.setVector(depthMapSpotLightFBO.getViewProjectionMatrix() * model, i);
            flashLightMVPMatrixSSBO.setVector(depthMapFlashLightFBO.getViewProjectionMatrix() * model, i);
        }

        NormalMatrixSSBO.updateAndBind();
        ModelViewMatrixSSBO.updateAndBind();
        MVPMatrixSSBO.updateAndBind();
        dirLightMVPMatrixSSBO.updateAndBind();
        spotLightMVPMatrixSSBO.updateAndBind();
        flashLightMVPMatrixSSBO.updateAndBind();

        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::cube.size()), GL_UNSIGNED_INT, 0, std::size(Data::cubePositions));

        /////////////////////////////////////
        ////// Model ////////////////////////
        /////////////////////////////////////

        multiLight.useShader();

        model = Global::getModelMatrix(glm::vec3(4.0f, 3.0f, 2.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        modelViewMatrix = Global::view * model;
        NormalMatrixSSBO.setVectorAndUpdateAndBind(glm::transpose(glm::inverse(modelViewMatrix)));
        ModelViewMatrixSSBO.setVectorAndUpdateAndBind(modelViewMatrix);
        MVPMatrixSSBO.setVectorAndUpdateAndBind(Global::projection * modelViewMatrix);
        dirLightMVPMatrixSSBO.setVectorAndUpdateAndBind(depthMapDirLightFBO.getViewProjectionMatrix() * model);
        spotLightMVPMatrixSSBO.setVectorAndUpdateAndBind(depthMapSpotLightFBO.getViewProjectionMatrix() * model);
        flashLightMVPMatrixSSBO.setVectorAndUpdateAndBind(depthMapFlashLightFBO.getViewProjectionMatrix() * model);

        multiLight.setFloat("material.shininess", 256.0f);
        multiLight.setInt("material.emission", 0); // black - 'disable' emission with a black texture
        multiLight.setFloat("flashLight.emissionStrength", 0.0f);
        ourModel.Draw(multiLight);

        /////////////////////////////////////
        ////// Floor ////////////////////////
        /////////////////////////////////////

        glStencilMask(0xFF); // enable writing to the stencil buffer

        multiLight.useShader();
        floorVAO.bindVertexArray();
        multiLight.setFloat("material.shininess", 8.0f);
        multiLight.setInt("material.diffuse1", 4);
        multiLight.setInt("material.specular1", 0); // black
        multiLight.setFloat("material.emissionStrength", 0.0f); // 'disable' emission with zero strength
        multiLight.setFloat("flashLight.emissionStrength", 0.0f);

        model = Global::getModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(25.0f, 25.0f, 2.0f));
        modelViewMatrix = Global::view * model;
        NormalMatrixSSBO.setVectorAndUpdateAndBind(glm::transpose(glm::inverse(modelViewMatrix)));
        ModelViewMatrixSSBO.setVectorAndUpdateAndBind(modelViewMatrix);
        MVPMatrixSSBO.setVectorAndUpdateAndBind(Global::projection * modelViewMatrix);
        dirLightMVPMatrixSSBO.setVectorAndUpdateAndBind(depthMapDirLightFBO.getViewProjectionMatrix() * model);
        spotLightMVPMatrixSSBO.setVectorAndUpdateAndBind(depthMapSpotLightFBO.getViewProjectionMatrix() * model);
        flashLightMVPMatrixSSBO.setVectorAndUpdateAndBind(depthMapFlashLightFBO.getViewProjectionMatrix()* model);
        
        glDisable(GL_CULL_FACE); // disable because floor has no Z dimension, the underside IS the BACK_FACE
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::floor2.size()), GL_UNSIGNED_INT, 0, 1);
        glEnable(GL_CULL_FACE);

        glStencilMask(0x00); // disable writing to the stencil buffer

        /////////////////////////////////////
        ////// Skybox ///////////////////////
        /////////////////////////////////////

        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.useShader();
        skyboxShader.setMat4("viewProjectionMatrixTranslationRemoved", Global::projection * glm::mat4(glm::mat3(Global::view))); // remove translation from the view matrix (cast to mat3 and back to mat4)
        skyboxVAO.bindVertexArray();
        //cubemapTexture.bindTexture(); // No need to bind if there is just a single GL_TEXTURE_CUBE_MAP
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS); // set depth function back to default

        /////////////////////////////////////
        ////// Floor Outline ////////////////
        /////////////////////////////////////

        // Until order independent transparency is implemented, partly transparant objects need to be drawn last (even after the skybox)
        if (Global::drawOutline) {
            singleColor.useShader();

            if (floorOutlineAlpha >= 0.0f)
                floorOutlineAlpha += 0.01f;
            if (floorOutlineAlpha >= 1.0f)
                floorOutlineAlpha = 0.0f;
            glm::vec4 color{ 1.0f, 0.28f, 0.26f, 0.0f };
            color.w = floorOutlineAlpha;
            singleColor.setVec4("color", color);

            glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // only draw according to stencil buffer

            modelViewMatrix = Global::getModelViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0, 0.0, 0.0), glm::vec3(26.0, 26.0, 2.0));
            MVPMatrixSSBO.setVectorAndUpdateAndBind(Global::projection * modelViewMatrix);

            floorVAO.bindVertexArray();

            glDisable(GL_CULL_FACE); // disable because floor has no Z dimension, the underside IS the BACK_FACE
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(Data::floor2.size()), GL_UNSIGNED_INT, 0, 1);
            glEnable(GL_CULL_FACE);

            // De-init Stencil Buffer
            glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test again
        }
        //std::println("End Renderpass time: {}ms", (static_cast<float>(glfwGetTime()) - loadTime) * 1000);

        // Draw debug quad (toggle with Q)
        // Set texture sampler2D binding in Shader itself
        if (Global::debugQuadVisible) {
            VertexArray quadVAO;
            VertexBuffer quadVBO(sizeof(Data::framebuffer), &Data::framebuffer);
            VertexAttributeLayout quadLayout{};
            quadLayout.pushVertexAttributeLayout<float>(2); // 2 coordinates, not 3!
            quadLayout.pushVertexAttributeLayout<float>(2);
            quadVAO.addVertexAttributeLayout(quadVBO, quadLayout);
            Shader debugQuadShader("Shaders\\debugQuad.shader");
            debugQuadShader.useShader();
            // TODO eerste z = nu -1.0f zodat quad exact(?) op de voorgrond staat, waarom is dat -1.0?
            debugQuadShader.setMat4("model", Global::getModelMatrix(glm::vec3(0.6f, 0.6f, -1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.0f)));
            debugQuadShader.setBool("orthographic", depthMapFlashLightFBO.getOrthographic());
            debugQuadShader.setFloat("nearPlane", depthMapFlashLightFBO.getNearPlane());
            debugQuadShader.setFloat("farPlane", depthMapFlashLightFBO.getFarPlane());
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        if (!Global::paused) { // toggle with P
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