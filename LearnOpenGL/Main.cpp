#pragma once

#include "Buffers.h"
#include "Camera.h"
#include "Data.h"
#include "Global.h"
#include "Light.h"

#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

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
#include <memory> // for std::unique_ptr
#include <print>

int main()
{
    std::println("START Main");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
    //Global::cheap2Copy();
    Renderer renderer;
    renderer.initStencilBuffer();
    //renderer.createShaderDepthMapDirLight("Shaders\\depthMapDirLight.shader");
    //renderer.createShaderDepthMapSpotLight0("Shaders\\depthMapFlashLight.shader");
    //renderer.createShaderDepthMapSpotLight1("Shaders\\depthMapSpotLight.shader");
    renderer.createShaderSingleColor("Shaders\\singleColor.shader");
    renderer.createShaderSkybox("Shaders\\skybox.shader");
    renderer.createShaderFrustum("Shaders\\frustum.shader");
    renderer.createShaderDebugQuad("Shaders\\debugQuad.shader");
    //renderer.isRendererComplete();

    //Shader depthMapShader("Shaders\\depthMap.shader");

    Shader multiLight("Shaders\\multiLight.shader");
    Shader multiLightNormalMapping("Shaders\\multiLightNormalMapping.shader");

    /////////////////////////////////////
    ////// Lights ///////////////////////
    std::println("CREATE Lights");///////

    PointLight::pointLights.emplace_back(PointLight());
    PointLight::pointLights[0].setPosition({ 0.7f, 11.2f, 2.0f });
    PointLight::pointLights[0].setColor({ 1.0f, 0.0f, 1.0f });
    PointLight::pointLights[0].setStrength(1.0f);
    PointLight::pointLights[0].setConstant(1.0f);
    PointLight::pointLights[0].setLinear(0.032f);
    PointLight::pointLights[0].setQuadratic(0.09f);
    PointLight::pointLights[0].sendToShader(multiLight);
    PointLight::pointLights[0].sendToShader(multiLightNormalMapping);

    PointLight::pointLights.emplace_back(PointLight());
    PointLight::pointLights[1].setPosition({ 4.0f,  2.0f, -12.0f });
    PointLight::pointLights[1].setColor({ 1.0f, 1.0f, 1.0f });
    PointLight::pointLights[1].sendToShader(multiLight);
    PointLight::pointLights[1].sendToShader(multiLightNormalMapping);

    PointLight::pointLights.emplace_back(PointLight());
    PointLight::pointLights[2].setPosition({ -4.0f,  2.0f,  12.0f });
    PointLight::pointLights[2].setColor({ 0.0f, 1.0f, 0.0f });
    PointLight::pointLights[2].sendToShader(multiLight);
    PointLight::pointLights[2].sendToShader(multiLightNormalMapping);

    PointLight::pointLights.emplace_back(PointLight());
    PointLight::pointLights[3].setPosition({ 15.0f,  1.2f,  -3.0f });
    PointLight::pointLights[3].setColor({ 0.0f, 0.0f, 1.0f });
    PointLight::pointLights[3].setLinear(0.035f);
    PointLight::pointLights[3].setQuadratic(0.44f);
    PointLight::pointLights[3].sendToShader(multiLight);
    PointLight::pointLights[3].sendToShader(multiLightNormalMapping);

    /////////////////////////////////////
    ////// light / depthMap /////////////
    std::println("CREATE Light / DepthMap");

    // DirLight
    DirectionalLight sun;
    sun.setDirection({ 0.7f, 0.9f, 0.8f });
    sun.setColor({ 1.0f, 1.0f, 0.95f });
    sun.setStrength(0.25f);
    sun.setDepthMap(2);
    sun.setAmbient(0.3f);
    sun.sendToShader(multiLight);
    sun.sendToShader(multiLightNormalMapping);

    OrthographicCamera cameraDirLight(sun.getDirection(), -20.0f, 20.0f, -20.0f, 20.0f);

    // DirLight depthMap
    Texture depthMapDirLight(textureType::depthMap, 4096, 4096);
    //renderer.createFBODirLight(depthMapDirLight);
    //renderer.getFBODirLight()->setOrthographic(true);

    renderer.m_FBOShaderPair.emplace_back(std::make_unique<FrameBuffer>(depthMapDirLight), std::make_unique<Shader>("Shaders\\depthMapDirLight.shader"));
    renderer.m_FBOShaderPair[0].first->setOrthographic(true);

    cameraDirLight.setNearPlane(-15.0f);
    cameraDirLight.setFarPlane(35.0f);
    cameraDirLight.setViewMatrix(glm::lookAt(sun.getDirection(), sun.getDirection() - sun.getDirection(), glm::vec3(0.0f, 1.0f, 0.0f))); // TODO
    cameraDirLight.calculateProjectionMatrix();

    // SpotLight0 | [0] == flashlight
    glm::vec3 flashLightOffset{ 0.4f, -0.3f, -0.1f };
    SpotLight::spotLights.emplace_back(SpotLight());
    SpotLight::spotLights[0].setOn(false);
    SpotLight::spotLights[0].setPosition(Global::camera.getPosition() + flashLightOffset);
    SpotLight::spotLights[0].setDirection({ 0.0f, 0.0f, -1.0f });
    SpotLight::spotLights[0].setColor({ 1.0f, 1.0f, 1.0f });
    SpotLight::spotLights[0].setStrength(5.5f); // waarom zo zwak resultaat? Omdat het bereik te ver of juist te kort is?
    SpotLight::spotLights[0].setDepthMap(6);
    SpotLight::spotLights[0].setConstant(1.0f);
    SpotLight::spotLights[0].setLinear(0.045f);
    SpotLight::spotLights[0].setQuadratic(0.0075f);
    SpotLight::spotLights[0].setInnerCutOff(8.5f);
    SpotLight::spotLights[0].setOuterCutOff(12.5f);
    SpotLight::spotLights[0].setEmissionStrength(1.0f);
    SpotLight::spotLights[0].sendToShader(multiLight);
    SpotLight::spotLights[0].sendToShader(multiLightNormalMapping);

    Camera cameraSpotLight0(depthMapDirLight.getAspectRatio(), Global::cameraInitialPosition);
    SpotLight::spotLights[0].setCamera(&cameraSpotLight0);

    Texture depthMapSpotLight0(textureType::depthMap, 512, 512);
    //renderer.createFBOSpotLight0(depthMapSpotLight0);

    renderer.m_FBOShaderPair.emplace_back(std::make_unique<FrameBuffer>(depthMapSpotLight0), std::make_unique<Shader>("Shaders\\depthMapFlashLight.shader"));

    cameraSpotLight0.setFov(25.0f);
    cameraSpotLight0.setNearPlane(0.1f);
    cameraSpotLight0.setFarPlane(25.0f);

    // SpotLight1
    SpotLight::spotLights.emplace_back(SpotLight());
    SpotLight::spotLights[1].setPosition({0.0f, 0.0f, 0.0f});
    SpotLight::spotLights[1].setDirection({ 0.0f, -1.0f, 0.0f });
    SpotLight::spotLights[1].setColor({ 1.0f, 1.0f, 1.0f });
    SpotLight::spotLights[1].setStrength(1.2f);
    SpotLight::spotLights[1].setDepthMap(5);
    SpotLight::spotLights[1].setConstant(1.0f);
    SpotLight::spotLights[1].setLinear(0.014f);
    SpotLight::spotLights[1].setQuadratic(0.07f);
    SpotLight::spotLights[1].setInnerCutOff(36.0f);
    SpotLight::spotLights[1].setOuterCutOff(48.0f);
    SpotLight::spotLights[1].setEmissionStrength(0.0f);
    SpotLight::spotLights[1].sendToShader(multiLight);
    SpotLight::spotLights[1].sendToShader(multiLightNormalMapping);

    Camera cameraSpotLight1(depthMapSpotLight0.getAspectRatio(), SpotLight::spotLights[1].getPosition(), SpotLight::spotLights[1].getPosition() + glm::vec3(0.0f, -SpotLight::spotLights[1].getPosition().y, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)); // cameraPos + glm::vec3(0.0f, -cameraPos.y, 0.0f) == glm::vec3(cameraPos.x, 0.0f, cameraPos.z)
    SpotLight::spotLights[1].setCamera(&cameraSpotLight1);

    Texture depthMapSpotLight1(textureType::depthMap, 1024, 1024);
    //renderer.createFBOSpotLight1(depthMapSpotLight1);

    renderer.m_FBOShaderPair.emplace_back(std::make_unique<FrameBuffer>(depthMapSpotLight1), std::make_unique<Shader>("Shaders\\depthMapSpotLight.shader"));

    cameraSpotLight1.setFov((36.0f + 48.0f) * 1.15f);
    cameraSpotLight1.setNearPlane(0.1f);
    cameraSpotLight1.setFarPlane(10.0f);

    /////////////////////////////////////
    ////// Quad /////////////////////////
    std::println("CREATE Quad");///////

    Mesh quadMesh(Data::quad, Data::quadIndices);

    /////////////////////////////////////
    ////// Cubes ////////////////////////
    std::println("CREATE Cubes");////////

    Mesh cubeMesh(Data::cube, Data::cubeIndices);

    Material cubeMaterial{
        .shader{ multiLight },
        .diffuse1{ 8 },
        .specular1{ 9 },
        .normal1{ 7 },
        .shininess{ 32.0f },
        .emissionTexture{ 10 },
        .emissionStrength{ 0.5f },
        .lightEmissionMap{ 11 },
        .lightEmissionTexture{ 12 },
    };

    RenderObject cubeRO{ &cubeMesh, &cubeMaterial, std::ssize(Data::cubePositions) };
    cubeRO.instances = 1; // temp fix to make sure the BufferDataStore size == elementsize, instead of 10x (std::ssize(Data::cubePositions)
    cubeRO.addSSBO(24, sizeof(uberSSBO));
    cubeRO.instances = 10;
    renderer.m_renderVector.emplace_back(&cubeRO);

    //cubeRO.addSSBO(dirLightMVPMatrixBP, sizeof(glm::mat4));
    //cubeRO.addSSBO(flashLightMVPMatrixBP, sizeof(glm::mat4));
    //cubeRO.addSSBO(spotLightMVPMatrixBP, sizeof(glm::mat4));
    //cubeRO.addSSBO(normalMatrixBP, sizeof(glm::mat4));
    //cubeRO.addSSBO(modelViewMatrixBP, sizeof(glm::mat4));
    //cubeRO.addSSBO(MVPMatrixBP, sizeof(glm::mat4));

    /////////////////////////////////////
    ////// Floor ////////////////////////
    std::println("CREATE Floor");////////

    Mesh floorMesh(Data::floor, Data::floorIndices);

    Material floorMaterial{
        .shader{ multiLight },
        .diffuse1{ 4 },
        .specular1{ 0 },
        .normal1{ 7 },
        .shininess{ 8.0f },
        .emissionTexture{ 0 },
        .emissionStrength{ 0.0f },
        .lightEmissionMap{ 0 },
        .lightEmissionTexture{ 12 },
        .enableGL_CULL_FACE{ false },
    };

    RenderObject floorRO{ &floorMesh, &floorMaterial };


    // Model voor de renderloop berekenen, floor is statisch -> niet helemaal, de outline heeft eigen model, maar die is op basis van onderstaand model
    floorRO.modelTransform[0] = Global::calculateModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(25.0f, 25.0f, 1.0f));
    floorRO.drawOutline = true;
    floorRO.addSSBO(24, sizeof(uberSSBO));

    //floorRO.addSSBO(dirLightMVPMatrixBP, sizeof(glm::mat4));
    //floorRO.addSSBO(flashLightMVPMatrixBP, sizeof(glm::mat4));
    //floorRO.addSSBO(spotLightMVPMatrixBP, sizeof(glm::mat4));
    //floorRO.addSSBO(normalMatrixBP, sizeof(glm::mat4));
    //floorRO.addSSBO(modelViewMatrixBP, sizeof(glm::mat4));
    //floorRO.addSSBO(MVPMatrixBP, sizeof(glm::mat4));
    floorRO.addSSBO(singleColorBP, sizeof(glm::vec4)); // TODO only 1 element needed in this vector
    renderer.m_renderVector.emplace_back(&floorRO);

    ////////////////////////////////////
    ////// Models //////////////////////
    std::println("LOAD Model");/////////

    float loadTime{ static_cast<float>(glfwGetTime()) };

    //Shader ourModelShader("Shaders\\multiLight.shader");
    //Model backpackModel("Models/Hexagon/hexagon.obj"); // crash
    //Model backpackModel("Models/Backpack/backpack.obj");
    //Model ourModel("Models/Nanosuit/nanosuit.obj");
    //Model ourModel("Models/Cyborg/cyborg.obj");
    //Model ourModel("Models/Mars/planet.obj");
    //Model ourModel("Models/Rock/rock.obj");
    //Model ourModel("Models/Vampire/dancing_vampire.dae"); // crash
    //Model ourModel("FinalBaseMesh.obj"); // TODO laadt niet 100%

    Material modelMaterial{
        .shader{ multiLightNormalMapping },
        .diffuse1{ 0 },
        .specular1{ 0 },
        .normal1{ 7 },
        .shininess{ 666.0f },
        .emissionTexture{ 0 },
        .emissionStrength{ 0.0f },
        .lightEmissionMap{ 0 },
        .lightEmissionTexture{ 0 },
    };

    RenderObject modelRO{ nullptr, &modelMaterial};
    modelRO.model = std::make_unique<Model>("Models/Backpack/backpack.obj");

    modelRO.modelTransform[0] = Global::calculateModelMatrix(glm::vec3(4.0f, 3.0f, 2.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    modelRO.addSSBO(24, sizeof(uberSSBO));
    renderer.m_renderVector.emplace_back(&modelRO);

    //modelRO.addSSBO(dirLightMVPMatrixBP, sizeof(glm::mat4));
    //modelRO.addSSBO(flashLightMVPMatrixBP, sizeof(glm::mat4));
    //modelRO.addSSBO(spotLightMVPMatrixBP, sizeof(glm::mat4));
    //modelRO.addSSBO(normalMatrixBP, sizeof(glm::mat4));
    //modelRO.addSSBO(modelViewMatrixBP, sizeof(glm::mat4));
    //modelRO.addSSBO(MVPMatrixBP, sizeof(glm::mat4));

    // Lightcubes
    RenderObject lightCubeRO{ &cubeMesh, nullptr, 5 };

    lightCubeRO.drawShadow = false;

    //lightCubeRO.addSSBO(MVPMatrixBP, sizeof(glm::mat4));
    lightCubeRO.instances = 1;
    lightCubeRO.addSSBO(24, sizeof(uberSSBO));
    lightCubeRO.instances = 5;
    lightCubeRO.addSSBO(singleColorBP, sizeof(glm::vec4));

    // TODO deze kleur/info moet eigenlijk uit de Light zelf worden gehaald
    const std::vector<glm::vec4> lightCubeColors = {
    glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), // magenta
    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), // white
    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // green
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // blue
    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // red
    };

    lightCubeRO.ssbo[1]->updateFully(lightCubeColors, false); // false, want in de loop wordt dit pas geupload, nadat element 4 is gewijzigd
    //renderer.m_renderVector.emplace_back(&lightCubeRO);
    
    //Global::deltaTime = currentFrame - Global::lastFrame;
    //Global::lastFrame = currentFrame;
    std::println("Load time model: {} seconds", static_cast<float>(glfwGetTime()) - loadTime);

    /////////////////////////////////////////////////////////////////////////////////////
    // All textures get loaded and bound here
    // Set material uniforms to the correct texture unit / values before each draw call
    std::println("AI TEXTURE ASSET MANAGER ***********************");
    /* 00 GL_TEXTURE_CUBE_MAP */ Texture cubemapTexture(Data::skybox1Faces);
    /* 00 */ Texture blackTexture(0x00000000);
    /* 01 */ Texture whiteTexture(0xffffffff);
    /* 02 */ 
    /* 03 */ Texture flashLightTexture("Textures\\flashlight.jpg");
    /* 04 */ Texture floorTexture("Textures\\floor.jpg");
    /* 05 */
    /* 06 */
    /* 07 */ Texture normalUpTexture("Textures\\normal_up.jpg", false); //0x807fffff); // TODO single color?
    /* 08 */ Texture cubeDiffuse("Textures\\container2.png");
    /* 09 */ Texture cubeSpecular("Textures\\container2_specular.png");
    /* 10 */ Texture cubeEmission("Textures\\emission.png");
    /* 11 */ Texture flashLightEmissionMapTexture("Textures\\container2_emission.png");
    /* 12 */ Texture flashLightEmissionTexture("Textures\\matrix.jpg");
    /* 13 */
    /* 14 */
    /* 15 */
    /* 16 - 31 */// Reserved for Model::Draw
    /////////////////////////////////////////////////////////////////////////////////////
    /* 00 GL_TEXTURE_CUBE_MAP */ cubemapTexture.bind(0);
    /* 00 */ blackTexture.bind(0);
    /* 01 */ whiteTexture.bind(1);
    /* 02 */ depthMapDirLight.bind(2);
    /* 03 */ flashLightTexture.bind(3);
    /* 04 */ floorTexture.bind(4);
    /* 05 */ depthMapSpotLight1.bind(5); // TODO omdraaien 5 en 6
    /* 06 */ depthMapSpotLight0.bind(6);
    /* 07 */ normalUpTexture.bind(7);
    /* 08 */ cubeDiffuse.bind(8);
    /* 09 */ cubeSpecular.bind(9);
    /* 10 */ cubeEmission.bind(10);
    /* 11 */ flashLightEmissionMapTexture.bind(11);
    /* 12 */ flashLightEmissionTexture.bind(12);
    /* 13 */
    /* 14 */
    /* 15 */
    /* 16 - 31 */// Reserved for Model::Draw
    std::println("************************************************");
    /////////////////////////////////////////////////////////////////////////////////////

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

        /////////////////////////////////////////////////////////////////////////////////////
        // Start processInput ///////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        glfwPollEvents();
        Global::processInput(window);

        if (!Global::isFlashLightOnUpdated) {
            SpotLight::spotLights[0].toggle(multiLight, multiLightNormalMapping);
            Global::isFlashLightOnUpdated = true;
        }

        /////////////////////////////////////////////////////////////////////////////////////
        // Start UpdateGame /////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        // Lights
        if (SpotLight::spotLights[0].getOn())
            Global::applyCameraOffset(SpotLight::spotLights[0].getCamera(), flashLightOffset.x, flashLightOffset.y, flashLightOffset.z);

        sun.updateDirectionInViewSpace(multiLight);
        sun.updateDirectionInViewSpace(multiLightNormalMapping);

        for (auto& pointLight : PointLight::pointLights) {
            pointLight.updatePositionInViewSpace(multiLight);
            pointLight.updatePositionInViewSpace(multiLightNormalMapping);
        }

        SpotLight::spotLights[1].updateDirectionInViewSpace(multiLight);
        SpotLight::spotLights[1].updateDirectionInViewSpace(multiLightNormalMapping);
        SpotLight::spotLights[1].setPosition({ 3.0f * static_cast<float>(sin(glfwGetTime())), 6.5f, static_cast<float>(4.5f * cos(glfwGetTime())) });
        SpotLight::spotLights[1].updatePositionInViewSpace(multiLight);
        SpotLight::spotLights[1].updatePositionInViewSpace(multiLightNormalMapping);
        SpotLight::spotLights[1].setColor({ static_cast<float>(sin(glfwGetTime() * 0.25f)), static_cast<float>(sin(glfwGetTime() * 0.50f)), static_cast<float>(sin(glfwGetTime() * 0.75f)) });
        SpotLight::spotLights[1].updateColor(multiLight);
        SpotLight::spotLights[1].updateColor(multiLightNormalMapping);

        // Calculate dynamic models/transforms and SSBO's
        // Cube
        for (auto i = 0; i < std::ssize(Data::cubePositions); i++)
        {
            assert(std::size(Data::cubePositions) <= cubeRO.instances && "Loop will create more instances then ssbo can hold");

            cubeRO.modelTransform[i] = glm::translate(glm::mat4(1.0f), Data::cubePositions[i]);
            if (i == 2 || i == 5 || i == 8) {
                float angle = 25.0f + (15 * i);
                cubeRO.modelTransform[i] = glm::rotate(cubeRO.modelTransform[i], (float)glfwGetTime() * glm::radians(100.0f) * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            }
            if (i == 3) { // wall
                cubeRO.modelTransform[i] = glm::translate(cubeRO.modelTransform[i], glm::vec3(-5.0f, 0.0f, -3.0f));
                cubeRO.modelTransform[i] = glm::scale(cubeRO.modelTransform[i], glm::vec3(20.0f, 20.0f, 1.0f));
            }
            //if (i == 9) { // floor
            //    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            //    model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
            //}

            cubeRO.modelTransform[i] = glm::scale(cubeRO.modelTransform[i], glm::vec3(0.5f, 0.5f, 0.5f));
        }

        // Calculate SSBO's, and upload them to their buffers
        //for (auto i = 0; i < std::ssize(cubeRO.model); i++) {
        //    Global::modelViewMatrixTemp = Global::camera.getViewMatrix() * cubeRO.model[i];
        //    cubeRO.ssbo[dirLightMVPMatrixSSBO]->updateSubset(cameraDirLight.getViewProjectionMatrix() * cubeRO.model[i], i);
        //    cubeRO.ssbo[flashLightMVPMatrixSSBO]->updateSubset(cameraFlashLight.getViewProjectionMatrix() * cubeRO.model[i], i);
        //    cubeRO.ssbo[spotLightMVPMatrixSSBO]->updateSubset(cameraSpotLight.getViewProjectionMatrix() * cubeRO.model[i], i);
        //    cubeRO.ssbo[normalMatrixSSBO]->updateSubset(glm::transpose(glm::inverse(Global::modelViewMatrixTemp)), i);
        //    cubeRO.ssbo[modelViewMatrixSSBO]->updateSubset(Global::modelViewMatrixTemp, i);
        //    cubeRO.ssbo[MVPMatrixSSBO]->updateSubset(Global::camera.getProjectionMatrix() * Global::modelViewMatrixTemp, i);
        //}
        //for (auto i = 0; i < std::ssize(cubeRO.ssbo); i++) {
        //    cubeRO.ssbo[i]->uploadFully();
        //}

        uberSSBO uberSSBOCache{};

        for (auto i = 0; i < std::ssize(cubeRO.modelTransform); i++) {
            Global::modelViewMatrixTemp = Global::camera.getViewMatrix() * cubeRO.modelTransform[i];
            uberSSBOCache.dirLightMVPMatrix[i] = cameraDirLight.getViewProjectionMatrix() * cubeRO.modelTransform[i];
            uberSSBOCache.flashLightMVPMatrix[i] = SpotLight::spotLights[0].getCamera()->getViewProjectionMatrix() * cubeRO.modelTransform[i];
            uberSSBOCache.spotLightMVPMatrix[i] = SpotLight::spotLights[1].getCamera()->getViewProjectionMatrix() * cubeRO.modelTransform[i];
            uberSSBOCache.normalMatrix[i] = glm::transpose(glm::inverse(Global::modelViewMatrixTemp));
            uberSSBOCache.modelViewMatrix[i] = Global::modelViewMatrixTemp;
            uberSSBOCache.MVPMatrix[i] = Global::camera.getProjectionMatrix() * Global::modelViewMatrixTemp;
        }
        cubeRO.ssbo[0]->updateSubset(uberSSBOCache, 0, true);
        //cubeRO.ssbo[0]->updateFully(temp);
        //cubeRO.ssbo[0]->uploadFully();
        //temp = {}; // clears the temp, but ended up not needed

        // Floor
        //for (auto i = 0; i < std::ssize(floorRO.model); i++) {
        //    Global::modelViewMatrixTemp = Global::camera.getViewMatrix() * floorRO.model[i];
        //    floorRO.ssbo[dirLightMVPMatrixSSBO]->updateSubset(cameraDirLight.getViewProjectionMatrix() * floorRO.model[i], i);
        //    floorRO.ssbo[flashLightMVPMatrixSSBO]->updateSubset(cameraFlashLight.getViewProjectionMatrix() * floorRO.model[i], i);
        //    floorRO.ssbo[spotLightMVPMatrixSSBO]->updateSubset(cameraSpotLight.getViewProjectionMatrix() * floorRO.model[i], i);
        //    floorRO.ssbo[normalMatrixSSBO]->updateSubset(glm::transpose(glm::inverse(Global::modelViewMatrixTemp)), i);
        //    floorRO.ssbo[modelViewMatrixSSBO]->updateSubset(Global::modelViewMatrixTemp, i);
        //    floorRO.ssbo[MVPMatrixSSBO]->updateSubset(Global::camera.getProjectionMatrix() * Global::modelViewMatrixTemp, i);
        //}
        //for (auto i = 0; i < std::ssize(floorRO.ssbo); i++) {
        //    floorRO.ssbo[i]->uploadFully();
        //}

        for (auto i = 0; i < std::ssize(floorRO.modelTransform); i++) {
            Global::modelViewMatrixTemp = Global::camera.getViewMatrix() * floorRO.modelTransform[i];
            uberSSBOCache.dirLightMVPMatrix[i] = cameraDirLight.getViewProjectionMatrix() * floorRO.modelTransform[i];
            uberSSBOCache.flashLightMVPMatrix[i] = SpotLight::spotLights[0].getCamera()->getViewProjectionMatrix() * floorRO.modelTransform[i];
            uberSSBOCache.spotLightMVPMatrix[i] = SpotLight::spotLights[1].getCamera()->getViewProjectionMatrix() * floorRO.modelTransform[i];
            uberSSBOCache.normalMatrix[i] = glm::transpose(glm::inverse(Global::modelViewMatrixTemp));
            uberSSBOCache.modelViewMatrix[i] = Global::modelViewMatrixTemp;
            uberSSBOCache.MVPMatrix[i] = Global::camera.getProjectionMatrix() * Global::modelViewMatrixTemp;

            //floorRO.ssbo[0]->updateSubset(temp, i);
        }
        floorRO.ssbo[0]->updateSubset(uberSSBOCache, 0, false);
        //floorRO.ssbo[0]->updateFully(temp);
        floorRO.ssbo[0]->uploadFully();
        //temp = {};

        // Model
        //for (auto i = 0; i < std::ssize(modelRO.model); i++) {
        //    Global::modelViewMatrixTemp = Global::camera.getViewMatrix() * modelRO.model[i];
        //    modelRO.ssbo[dirLightMVPMatrixSSBO]->updateSubset(cameraDirLight.getViewProjectionMatrix() * modelRO.model[i], i);
        //    modelRO.ssbo[flashLightMVPMatrixSSBO]->updateSubset(cameraFlashLight.getViewProjectionMatrix() * modelRO.model[i], i);
        //    modelRO.ssbo[spotLightMVPMatrixSSBO]->updateSubset(cameraSpotLight.getViewProjectionMatrix() * modelRO.model[i], i);
        //    modelRO.ssbo[normalMatrixSSBO]->updateSubset(glm::transpose(glm::inverse(Global::modelViewMatrixTemp)), i);
        //    modelRO.ssbo[modelViewMatrixSSBO]->updateSubset(Global::modelViewMatrixTemp, i);
        //    modelRO.ssbo[MVPMatrixSSBO]->updateSubset(Global::camera.getProjectionMatrix() * Global::modelViewMatrixTemp, i);
        //}
        //for (auto i = 0; i < std::ssize(modelRO.ssbo); i++) {
        //    modelRO.ssbo[i]->uploadFully();
        //}

        for (auto i = 0; i < std::ssize(modelRO.modelTransform); i++) {
            Global::modelViewMatrixTemp = Global::camera.getViewMatrix() * modelRO.modelTransform[i];
            uberSSBOCache.dirLightMVPMatrix[i] = cameraDirLight.getViewProjectionMatrix() * modelRO.modelTransform[i];
            uberSSBOCache.flashLightMVPMatrix[i] = SpotLight::spotLights[0].getCamera()->getViewProjectionMatrix() * modelRO.modelTransform[i];
            uberSSBOCache.spotLightMVPMatrix[i] = SpotLight::spotLights[1].getCamera()->getViewProjectionMatrix() * modelRO.modelTransform[i];
            uberSSBOCache.normalMatrix[i] = glm::transpose(glm::inverse(Global::modelViewMatrixTemp));
            uberSSBOCache.modelViewMatrix[i] = Global::modelViewMatrixTemp;
            uberSSBOCache.MVPMatrix[i] = Global::camera.getProjectionMatrix() * Global::modelViewMatrixTemp;

            //modelRO.ssbo[0]->updateSubset(temp, i);
        }
        modelRO.ssbo[0]->updateSubset(uberSSBOCache, 0, false);
        //modelRO.ssbo[0]->updateFully(temp);
        modelRO.ssbo[0]->uploadUntilSubset();
        //temp = {};

        // LightCube
        // pointlights - 4 vaste LightCubes
        for (auto i = 0; i < getPointLightCount(); i++) {
            assert(getPointLightCount() <= lightCubeRO.instances && "Loop will create more instances then ssbo can hold");
            lightCubeRO.modelTransform[i] = Global::calculateModelMatrix(glm::vec3(PointLight::pointLights[i].getPosition()), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)); // you could move this to line below
            uberSSBOCache.MVPMatrix[i] = Global::camera.getViewProjectionMatrix() * lightCubeRO.modelTransform[i];
            //lightCubeRO.ssbo[0]->updateSubset(Global::camera.getViewProjectionMatrix() * lightCubeRO.model[i], i, false);
        }

        // #5, element 4, de draaiende lightcube
        lightCubeRO.modelTransform[4] = Global::calculateModelMatrix(SpotLight::spotLights[1].getPosition(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)); // you could move this to line below
        uberSSBOCache.MVPMatrix[4] = Global::camera.getViewProjectionMatrix() * lightCubeRO.modelTransform[4];
        //lightCubeRO.ssbo[0]->updateSubset(Global::camera.getViewProjectionMatrix() * lightCubeRO.model[4], 4, false);

        lightCubeRO.ssbo[0]->updateFully(uberSSBOCache, true);

        lightCubeRO.ssbo[1]->updateSubset(glm::vec4(SpotLight::spotLights[1].getColor(), 1.0f), 4, false);
        lightCubeRO.ssbo[1]->uploadFully();

        /////////////////////////////////////////////////////////////////////////////////////
        // Start Render /////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        // Depthpasses and normal renderpass
        renderer.goRender();

        // Skybox
        renderer.goRenderSkybox(cubeMesh);

        // Floor Outline
        renderer.goRenderOutline();

        // Draw frustum - toggle with K
        if (Global::frustumVisible) {
            //renderer.drawFrustum(cubeMesh, cameraDirLight.getViewProjectionMatrix());
            renderer.drawFrustum(cubeMesh, cameraSpotLight0.getViewProjectionMatrix());
            //renderer.drawFrustum(cubeMesh, cameraSpotLight1.getViewProjectionMatrix());
        }

        // Draw debug quad - toggle with Q
        if (Global::debugQuadVisible) {
            // Set texture sampler2D binding in Shader itself + set orthographic in function + set corresponding Camera below
            //renderer.drawDebugQuad(quadMesh, cameraDirLight); //  binding 2
            renderer.drawDebugQuad(quadMesh, cameraSpotLight0); // binding 6
            //renderer.drawDebugQuad(quadMesh, cameraSpotLight1); //  binding 5
        }

        if (!Global::paused) { // toggle with P
            glfwSwapBuffers(window);
        }
    }
    Global::glCheckError();
    glfwTerminate();
    Global::glClearError(); // TODO glfwTerminate() produce errors
    return 0;
}