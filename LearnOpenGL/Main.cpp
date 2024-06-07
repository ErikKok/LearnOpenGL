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

    Global::initStencilBuffer();

    Global::glCheckError();
    //Global::cheap2Copy();
    Renderer renderer;
    renderer.createShaderDepthMapDirLight("Shaders\\depthMapDirLight.shader");
    renderer.createShaderDepthMapSpotLight("Shaders\\depthMapSpotLight.shader");
    renderer.createShaderDepthMapFlashLight("Shaders\\depthMapFlashLight.shader");
    renderer.createShaderSingleColor("Shaders\\singleColor.shader");
    renderer.createShaderSkybox("Shaders\\skybox.shader");
    renderer.createShaderFrustum("Shaders\\frustum.shader");
    renderer.createShaderDebugQuad("Shaders\\debugQuad.shader");
    renderer.isRendererComplete();

    Shader multiLight("Shaders\\multiLight.shader");
    Shader multiLightNormalMapping("Shaders\\multiLightNormalMapping.shader");

    /////////////////////////////////////
    ////// Lights ///////////////////////
    std::println("CREATE Lights");///////

    // PointLight (max amount hard coded in shader TODO)
    glm::vec3 pointLightPositions[] = { // World space
        glm::vec3(0.7f, 11.2f,   2.0f),
        glm::vec3(4.0f,  2.0f, -12.0f),
        glm::vec3(-4.0f,  2.0f,  12.0f),
        glm::vec3(15.0f,  1.2f,  -3.0f),
    };

    const std::vector<testSSBO> pointLightColors2 = {
        {1, 1.0f, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)},
        { 2, 10.0f, {1.0f, 1.0f, 1.0f, 1.0f} }, // alternatieve notatie
        {0, 0.5f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
        {8, 1.8f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)},
        {42, 0.0123456f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
    };

    const std::vector<glm::vec4> pointLightColors = {
    glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), // magenta
    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), // white
    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // green
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // blue
    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // red
    };

    // TODO use variable length arrays through SSBO
    // Removes the need to manually set the pointLightsCount here AND in the shader
    // See -> https://computergraphics.stackexchange.com/questions/5323/dynamic-array-in-glsl
    multiLight.useShader();
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
 
    multiLightNormalMapping.useShader();
    multiLightNormalMapping.setInt("pointLightsCount", 4);

    multiLightNormalMapping.setVec3("pointLights[0].color", pointLightColors[0]);  // red, distance 50
    multiLightNormalMapping.setFloat("pointLights[0].constant", 1.0f);
    multiLightNormalMapping.setFloat("pointLights[0].linear", 0.09f);
    multiLightNormalMapping.setFloat("pointLights[0].quadratic", 0.032f);
    multiLightNormalMapping.setFloat("pointLights[0].strength", 1.0f);

    multiLightNormalMapping.setVec3("pointLights[1].color", pointLightColors[1]);  // green, distance 50
    multiLightNormalMapping.setFloat("pointLights[1].constant", 1.0f);
    multiLightNormalMapping.setFloat("pointLights[1].linear", 0.09f);
    multiLightNormalMapping.setFloat("pointLights[1].quadratic", 0.032f);
    multiLightNormalMapping.setFloat("pointLights[1].strength", 2.0f);

    multiLightNormalMapping.setVec3("pointLights[2].color", pointLightColors[2]);  // blue, distance 325
    multiLightNormalMapping.setFloat("pointLights[2].constant", 1.0f);
    multiLightNormalMapping.setFloat("pointLights[2].linear", 0.09f);
    multiLightNormalMapping.setFloat("pointLights[2].quadratic", 0.032f);
    multiLightNormalMapping.setFloat("pointLights[2].strength", 3.0f);

    multiLightNormalMapping.setVec3("pointLights[3].color", pointLightColors[3]);  // white, distance 13
    multiLightNormalMapping.setFloat("pointLights[3].constant", 1.0f);
    multiLightNormalMapping.setFloat("pointLights[3].linear", 0.35f);
    multiLightNormalMapping.setFloat("pointLights[3].quadratic", 0.44f);
    multiLightNormalMapping.setFloat("pointLights[3].strength", 1.0f);

    /////////////////////////////////////
    ////// light / depthMap /////////////
    std::println("CREATE Light / DepthMap");

    // DirLight
    DirectionalLight sun;
    sun.setDirection(0.7f, 0.9f, 0.8f); // TODO light position == camera position == needs to sync, or delete 1
    sun.setColor(1.0f, 1.0f, 0.95f);
    sun.setStrength(0.25f);
    sun.setDepthMap(2);
    sun.setAmbient(0.3f);
    sun.sendToShader(multiLight);
    sun.sendToShader(multiLightNormalMapping);

    OrthographicCamera cameraDirLight(sun.getDirection(), -20.0f, 20.0f, -20.0f, 20.0f);

    // DirLight depthMap
    Texture depthMapDirLight(textureType::depthMap, 4096, 4096);
    FrameBuffer depthMapDirLightFBO(depthMapDirLight);
    depthMapDirLightFBO.setOrthographic(true); // TODO needs to sync with camera type
    cameraDirLight.setNearPlane(-15.0f);
    cameraDirLight.setFarPlane(35.0f);
    cameraDirLight.setViewMatrix(glm::lookAt(sun.getDirection(), sun.getDirection() - sun.getDirection(), glm::vec3(0.0f, 1.0f, 0.0f))); // TODO
    cameraDirLight.calculateProjectionMatrix();

    // SpotLight
    SpotLight spotLight;
    spotLight.setPosition(0.0f, -1.0f, 0.0f); // TODO light position == camera position == needs to sync, or delete 1
    spotLight.setDirection(0.0f, -1.0f, 0.0f);
    spotLight.setColor(1.0f, 1.0f, 1.0f);
    spotLight.setStrength(1.2f);
    spotLight.setDepthMap(5);
    spotLight.setConstant(1.0f);
    spotLight.setLinear(0.014f);
    spotLight.setQuadratic(0.07f);
    spotLight.setInnerCutOff(36.0f);
    spotLight.setOuterCutOff(48.0f);
    spotLight.sendToShader(multiLight);
    spotLight.sendToShader(multiLightNormalMapping);

    // TODO get aspectratio from depthmap texture
    Camera cameraSpotLight(1.0f, spotLight.getPosition(), spotLight.getPosition() + glm::vec3(0.0f, -spotLight.getPosition().y, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)); // cameraPos + glm::vec3(0.0f, -cameraPos.y, 0.0f) == glm::vec3(cameraPos.x, 0.0f, cameraPos.z)

    // SpotLight depthMap
    Texture depthMapSpotLight(textureType::depthMap, 1024, 1024);
    FrameBuffer depthMapSpotLightFBO(depthMapSpotLight);
    cameraSpotLight.setFov((36.0f + 48.0f) * 1.15f); // InnerCutOff + OuterCutOff + 15% for attenuation
    cameraSpotLight.setNearPlane(0.1f);
    cameraSpotLight.setFarPlane(10.0f);

    // FlashLight
    FlashLight flashLight;
    flashLight.setOn(false);
    flashLight.setColor(1.0f, 1.0f, 1.0f);
    flashLight.setStrength(5.5f); // waarom zo zwak resultaat? Omdat het bereik te ver of juist te kort is?
    flashLight.setDepthMap(6);
    flashLight.setConstant(1.0f);
    flashLight.setLinear(0.045f);
    flashLight.setQuadratic(0.0075f);
    flashLight.setInnerCutOff(8.5f);
    flashLight.setOuterCutOff(12.5f);
    flashLight.setEmissionStrength(0.8f);
    flashLight.sendToShader(multiLight); 
    flashLight.sendToShader(multiLightNormalMapping);

    // FlashLight depthMap
    Texture depthMapFlashLight(textureType::depthMap, 1920, 1080);
    FrameBuffer depthMapFlashLightFBO(depthMapFlashLight);
    Global::cameraFlashLight.setFov(60.0f); // too wide, but otherwise does not work ok while zooming
    Global::cameraFlashLight.setNearPlane(0.1f);
    Global::cameraFlashLight.setFarPlane(100.0f);

    /////////////////////////////////////
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

    /////////////////////////////////////
    ////// Quad /////////////////////////
    //std::println("CREATE Quad");///////

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
        .emission{ 10 },
        .emissionStrength{ 0.5f },
        .shininess{ 32.0f },
        .flashLightEmissionMap{ 11 },
        .flashLightEmissionTexture{ 12 },
    };

    RenderObject cubeRO{ &cubeMesh, &cubeMaterial };

    // set instances 
    cubeRO.instances = std::ssize(Data::cubePositions);
    // each instance must have it's own model, TODO check?
    cubeRO.model.resize(cubeRO.instances);

    cubeRO.ssbo.resize(6); // amount of SSBOs to hold // TODO can this be automated?
    cubeRO.ssbo[0] = std::make_unique<ShaderStorageBuffer>(5, cubeRO.instances); // dirLightMVPMatrixSSBO       // TODO volgorde klopt niet
    cubeRO.ssbo[1] = std::make_unique<ShaderStorageBuffer>(7, cubeRO.instances); // flashLightMVPMatrixSSBO
    cubeRO.ssbo[2] = std::make_unique<ShaderStorageBuffer>(6, cubeRO.instances); // spotLightMVPMatrixSSBO
    cubeRO.ssbo[3] = std::make_unique<ShaderStorageBuffer>(2, cubeRO.instances); // NormalMatrixSSBO
    cubeRO.ssbo[4] = std::make_unique<ShaderStorageBuffer>(3, cubeRO.instances); // ModelViewMatrixSSBO
    cubeRO.ssbo[5] = std::make_unique<ShaderStorageBuffer>(4, cubeRO.instances); // MVPMatrixSSBO

    /////////////////////////////////////
    ////// Floor ////////////////////////
    std::println("CREATE Floor");////////

    Mesh floorMesh(Data::floor, Data::floorIndices);

    float floorOutlineAlpha{ 0.0f };

    Material floorMaterial{
        .shader{ multiLight },
        .diffuse1{ 4 },
        .specular1{ 0 },
        .normal1{ 7 },
        .emission{ 0 },
        .emissionStrength{ 0.0f },
        .shininess{ 8.0f },
        .flashLightEmissionMap{ 0 },
        .flashLightEmissionTexture{ 12 },
    };

    RenderObject floorRO{ &floorMesh, &floorMaterial };

    // Model voor de renderloop berekenen, floor is statisch -> niet helemaal, de outline heeft eigen model, maar die is op basis van onderstaand model
    floorRO.model.resize(1);
    floorRO.model[0] = Global::getModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(25.0f, 25.0f, 1.0f));

    floorRO.ssbo.resize(7); // amount of SSBO's to hold
    floorRO.ssbo[0] = std::make_unique<ShaderStorageBuffer>(5, floorRO.instances); // dirLightMVPMatrixSSBO
    floorRO.ssbo[1] = std::make_unique<ShaderStorageBuffer>(7, floorRO.instances); // flashLightMVPMatrixSSBO
    floorRO.ssbo[2] = std::make_unique<ShaderStorageBuffer>(6, floorRO.instances); // spotLightMVPMatrixSSBO
    floorRO.ssbo[3] = std::make_unique<ShaderStorageBuffer>(2, floorRO.instances); // NormalMatrixSSBO
    floorRO.ssbo[4] = std::make_unique<ShaderStorageBuffer>(3, floorRO.instances); // ModelViewMatrixSSBO
    floorRO.ssbo[5] = std::make_unique<ShaderStorageBuffer>(4, floorRO.instances); // MVPMatrixSSBO
    floorRO.ssbo[6] = std::make_unique<ShaderStorageBuffer>(20); // singleColor

    BufferDataStore singleColorssboLayoutFloor(floorRO.ssbo[6]->getId(), glm::vec4(1.0f, 0.28f, 0.26f, 1.0f));
    singleColorssboLayoutFloor.createAndInitializeImmutableDataStore();

    ////////////////////////////////////
    ////// Models //////////////////////
    std::println("LOAD Model");/////////

    float loadTime{ static_cast<float>(glfwGetTime()) };

    //Shader ourModelShader("Shaders\\multiLight.shader");
    Model backpackModel("Models/Backpack/backpack.obj");
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
        .emission{ 0 },
        .emissionStrength{ 0.0f },
        .shininess{ 666.0f },
        .flashLightEmissionMap{ 0 },
        .flashLightEmissionTexture{ 0 },
    };

    RenderObject modelRO{ nullptr, &modelMaterial}; // TODO Feed mesh[0] from model

    modelRO.model.resize(1);
    modelRO.model[0] = Global::getModelMatrix(glm::vec3(4.0f, 3.0f, 2.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    modelRO.ssbo.resize(6); // amount of SSBO's to hold
    modelRO.ssbo[0] = std::make_unique<ShaderStorageBuffer>(5, modelRO.instances); // dirLightMVPMatrixSSBO
    modelRO.ssbo[1] = std::make_unique<ShaderStorageBuffer>(7, modelRO.instances); // flashLightMVPMatrixSSBO
    modelRO.ssbo[2] = std::make_unique<ShaderStorageBuffer>(6, modelRO.instances); // spotLightMVPMatrixSSBO
    modelRO.ssbo[3] = std::make_unique<ShaderStorageBuffer>(2, modelRO.instances); // NormalMatrixSSBO
    modelRO.ssbo[4] = std::make_unique<ShaderStorageBuffer>(3, modelRO.instances); // ModelViewMatrixSSBO
    modelRO.ssbo[5] = std::make_unique<ShaderStorageBuffer>(4, modelRO.instances); // MVPMatrixSSBO

    // Lightcubes

    RenderObject lightCubeRO{ &cubeMesh };
    lightCubeRO.instances = 5;
    lightCubeRO.model.resize(5); // 5 lights
    lightCubeRO.ssbo.resize(2);
    lightCubeRO.ssbo[0] = std::make_unique<ShaderStorageBuffer>(4, lightCubeRO.instances); // MVPMatrixSSBO
    lightCubeRO.ssbo[1] = std::make_unique<ShaderStorageBuffer>(20); // singleColor

    BufferDataStore singleColorssboLayoutLightCubes(lightCubeRO.ssbo[1]->getId(), pointLightColors);
    singleColorssboLayoutLightCubes.createAndInitializeImmutableDataStore();

    //Global::deltaTime = currentFrame - Global::lastFrame;
    //Global::lastFrame = currentFrame;
    std::println("Load time model: {} seconds", static_cast<float>(glfwGetTime()) - loadTime);

    /////////////////////////////////////////////////////////////////////////////////////
    // All textures get loaded and bind here
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
    /* 07 */ Texture normalUpTexture(0x807fffff);
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
    /* 05 */ depthMapSpotLight.bind(5);
    /* 06 */ depthMapFlashLight.bind(6);
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
        Global::clearStencilBuffer();
        Global::processInput(window);

        /////////////////////////////////////
        ////// Lights ///////////////////////
        /////////////////////////////////////

        // Transform dirLight direction to current View Space
        sun.updateDirectionInViewSpace(multiLight);

        multiLight.useShader();
        multiLight.setVec3("pointLights[0].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[0], 1.0)));  // red
        multiLight.setVec3("pointLights[1].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[1], 1.0)));  // green
        multiLight.setVec3("pointLights[2].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[2], 1.0)));  // blue
        multiLight.setVec3("pointLights[3].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[3], 1.0)));  // white

        // Transform Spotlight direction to current current View Space
        spotLight.updateDirectionInViewSpace(multiLight);
        // Calculate Spotlight position and transform to current View Space
        spotLight.setPosition(3.0f * static_cast<float>(sin(glfwGetTime())), 6.5f, static_cast<float>(4.5f * cos(glfwGetTime())));
        spotLight.updatePositionInViewSpace(multiLight);
        // Calculate Spotlight color
        spotLight.setColor(static_cast<float>(sin(glfwGetTime() * 0.25f)), static_cast<float>(sin(glfwGetTime() * 0.50f)), static_cast<float>(sin(glfwGetTime() * 0.75f)));
        spotLight.updateColor(multiLight);

        /////

        // Transform dirLight direction to current View Space
        sun.updateDirectionInViewSpace(multiLightNormalMapping);

        multiLightNormalMapping.useShader();
        multiLightNormalMapping.setVec3("pointLights[0].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[0], 1.0)));  // red
        multiLightNormalMapping.setVec3("pointLights[1].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[1], 1.0)));  // green
        multiLightNormalMapping.setVec3("pointLights[2].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[2], 1.0)));  // blue
        multiLightNormalMapping.setVec3("pointLights[3].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[3], 1.0)));  // white

        // Transform Spotlight direction to current View Space
        spotLight.updateDirectionInViewSpace(multiLightNormalMapping);
        // Calculate Spotlight position and transform to current View Space
        spotLight.setPosition(3.0f * static_cast<float>(sin(glfwGetTime())), 6.5f, static_cast<float>(4.5f * cos(glfwGetTime())));
        spotLight.updatePositionInViewSpace(multiLightNormalMapping);
        // Calculate Spotlight color
        spotLight.setColor(static_cast<float>(sin(glfwGetTime() * 0.25f)), static_cast<float>(sin(glfwGetTime() * 0.50f)), static_cast<float>(sin(glfwGetTime() * 0.75f)));
        spotLight.updateColor(multiLightNormalMapping);

        /////////////////////////////////////////////////////////////////////////////////////
        // Calculate dynamic models/transforms and SSBO's ///////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        // Cube
        for (int i = 0; i < std::ssize(Data::cubePositions); i++)
        {
            assert(std::size(Data::cubePositions) <= cubeRO.instances && "Loop will create more instances then ssbo can hold");

            cubeRO.model[i] = glm::translate(glm::mat4(1.0f), Data::cubePositions[i]);
            if (i == 2 || i == 5 || i == 8) {
                float angle = 25.0f + (15 * i);
                cubeRO.model[i] = glm::rotate(cubeRO.model[i], (float)glfwGetTime() * glm::radians(100.0f) * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            }
            if (i == 3) { // wall
                cubeRO.model[i] = glm::translate(cubeRO.model[i], glm::vec3(-5.0f, 0.0f, -3.0f));
                cubeRO.model[i] = glm::scale(cubeRO.model[i], glm::vec3(20.0f, 20.0f, 1.0f));
            }
            //if (i == 9) { // floor
            //    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            //    model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
            //}

            cubeRO.model[i] = glm::scale(cubeRO.model[i], glm::vec3(0.5f, 0.5f, 0.5f));
        }

        // Calculate SSBO's, and upload them to their buffers
        for (int i = 0; i < std::ssize(cubeRO.model); i++) {
            modelViewMatrix = Global::camera.getViewMatrix() * cubeRO.model[i];
            cubeRO.ssbo[0]->update(cameraDirLight.getViewProjectionMatrix() * cubeRO.model[i], i);
            cubeRO.ssbo[1]->update(Global::cameraFlashLight.getViewProjectionMatrix() * cubeRO.model[i], i);
            cubeRO.ssbo[2]->update(cameraSpotLight.getViewProjectionMatrix() * cubeRO.model[i], i);
            cubeRO.ssbo[3]->update(glm::transpose(glm::inverse(modelViewMatrix)), i);
            cubeRO.ssbo[4]->update(modelViewMatrix, i);
            cubeRO.ssbo[5]->update(Global::camera.getProjectionMatrix() * modelViewMatrix, i);
        }
        cubeRO.ssbo[0]->upload();
        cubeRO.ssbo[1]->upload();
        cubeRO.ssbo[2]->upload();
        cubeRO.ssbo[3]->upload();
        cubeRO.ssbo[4]->upload();
        cubeRO.ssbo[5]->upload();

        // Floor
        for (int i = 0; i < std::ssize(floorRO.model); i++) {
            modelViewMatrix = Global::camera.getViewMatrix() * floorRO.model[i];
            floorRO.ssbo[0]->update(cameraDirLight.getViewProjectionMatrix() * floorRO.model[i], i);
            floorRO.ssbo[1]->update(Global::cameraFlashLight.getViewProjectionMatrix() * floorRO.model[i], i);
            floorRO.ssbo[2]->update(cameraSpotLight.getViewProjectionMatrix() * floorRO.model[i], i);
            floorRO.ssbo[3]->update(glm::transpose(glm::inverse(modelViewMatrix)), i);
            floorRO.ssbo[4]->update(modelViewMatrix, i);
            floorRO.ssbo[5]->update(Global::camera.getProjectionMatrix() * modelViewMatrix, i);
        }
        floorRO.ssbo[0]->upload();
        floorRO.ssbo[1]->upload();
        floorRO.ssbo[2]->upload();
        floorRO.ssbo[3]->upload();
        floorRO.ssbo[4]->upload();
        floorRO.ssbo[5]->upload();

        // Model
        for (int i = 0; i < std::ssize(modelRO.model); i++) {
            modelViewMatrix = Global::camera.getViewMatrix() * modelRO.model[i];
            modelRO.ssbo[0]->update(cameraDirLight.getViewProjectionMatrix() * modelRO.model[i], i);
            modelRO.ssbo[1]->update(Global::cameraFlashLight.getViewProjectionMatrix() * modelRO.model[i], i);
            modelRO.ssbo[2]->update(cameraSpotLight.getViewProjectionMatrix() * modelRO.model[i], i);
            modelRO.ssbo[3]->update(glm::transpose(glm::inverse(modelViewMatrix)), i);
            modelRO.ssbo[4]->update(modelViewMatrix, i);
            modelRO.ssbo[5]->update(Global::camera.getProjectionMatrix() * modelViewMatrix, i);
        }
        modelRO.ssbo[0]->upload();
        modelRO.ssbo[1]->upload();
        modelRO.ssbo[2]->upload();
        modelRO.ssbo[3]->upload();
        modelRO.ssbo[4]->upload();
        modelRO.ssbo[5]->upload();

        /////////////////////////////////////////////////////////////////////////////////////
        // Start ShadowPass dirLight ////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        loadTime = static_cast<float>(glfwGetTime());

        renderer.setRenderPassActive(renderPassType::depthMapDirLight);

        depthMapDirLightFBO.startDepthMap(renderer.getShaderDepthMapDirLight());

        renderer.draw(cubeRO);
        renderer.draw(floorRO);
        renderer.drawModel(modelRO, backpackModel);

        depthMapDirLightFBO.stopDepthMap();
        //std::println("End ShadowPass dirLight time: {}ms", (static_cast<float>(glfwGetTime()) - loadTime) * 1000);

        /////////////////////////////////////////////////////////////////////////////////////
        // Start ShadowPass flashLight //////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        loadTime = static_cast<float>(glfwGetTime());

        renderer.setRenderPassActive(renderPassType::depthMapFlashLight);

        depthMapFlashLightFBO.startDepthMap(renderer.getShaderDepthMapFlashLight());

        renderer.draw(cubeRO);
        renderer.draw(floorRO);
        renderer.drawModel(modelRO, backpackModel);

        depthMapFlashLightFBO.stopDepthMap();
        //std::println("End ShadowPass spotLight time: {}ms", (static_cast<float>(glfwGetTime()) - loadTime) * 1000);

        /////////////////////////////////////////////////////////////////////////////////////
        // Start ShadowPass spotLight ///////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        loadTime = static_cast<float>(glfwGetTime());

        renderer.setRenderPassActive(renderPassType::depthMapSpotLight);

        depthMapSpotLightFBO.startDepthMap(renderer.getShaderDepthMapSpotLight());

        // TODO sync light position with camera classes
        glm::mat4 view = glm::lookAt(spotLight.getPosition(), spotLight.getPosition() + glm::vec3(0.0f, -spotLight.getPosition().y, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)); // cameraPos + glm::vec3(0.0f, -cameraPos.y, 0.0f) == glm::vec3(cameraPos.x, 0.0f, cameraPos.z)

        cameraSpotLight.setViewMatrix(view); // TODO

        renderer.draw(cubeRO);
        renderer.draw(floorRO);
        renderer.drawModel(modelRO, backpackModel);

        depthMapSpotLightFBO.stopDepthMap();
        //std::println("End ShadowPass spotLight time: {}ms", (static_cast<float>(glfwGetTime()) - loadTime) * 1000);

        /////////////////////////////////////////////////////////////////////////////////////
        // Start Render scene normal //////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        loadTime = static_cast<float>(glfwGetTime());

        renderer.setRenderPassActive(renderPassType::normal);

        glViewport(0, 0, Global::windowWidth, Global::windowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!Global::flashLightOnUpdated) {
            flashLight.toggle(multiLight, multiLightNormalMapping);
            Global::flashLightOnUpdated = true;
        }

        renderer.draw(cubeRO);
     
        glStencilMask(0xFF); // enable writing to the stencil buffer
        glDisable(GL_CULL_FACE); // disable because floor has no Z dimension, the underside IS the BACK_FACE
        renderer.draw(floorRO);
        glEnable(GL_CULL_FACE);
        glStencilMask(0x00); // disable writing to the stencil buffer

        renderer.drawModel(modelRO, backpackModel);

        // XYZ
        //renderer.drawXYZ(MVPMatrixSSBO); // Non-DSA

        ////// LightCube ////////////////////
        /////////////////////////////////////
        
        // pointlights - 4 vaste LightCubes
        for (int i = 0; i < std::size(pointLightPositions); i++) {
            assert(std::size(pointLightPositions) <= ArrayCountSSBO && "Loop will create more instances then ssbo can hold");
            lightCubeRO.model[i] = Global::getModelMatrix(glm::vec3(pointLightPositions[i]), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)); // you could move this to line below
            lightCubeRO.ssbo[0]->update(Global::camera.getViewProjectionMatrix() * lightCubeRO.model[i], i);
        }

        // #5, element 4, de draaiende lightcube
        lightCubeRO.model[4] = Global::getModelMatrix(spotLight.getPosition(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)); // you could move this to line below
        lightCubeRO.ssbo[0]->update(Global::camera.getViewProjectionMatrix() * lightCubeRO.model[4], 4);
        singleColorssboLayoutLightCubes.updateSubset(glm::vec4(spotLight.getColor(), 1.0f), 4);

        lightCubeRO.ssbo[0]->upload();
        renderer.drawSingleColor(lightCubeRO);

        // Skybox
        renderer.drawSkybox(cubeMesh);

        ////// Floor Outline ////////////////
        /////////////////////////////////////

        // Until order independent transparency is implemented, partly transparant objects need to be drawn last (even after the skybox)
        if (Global::drawOutline) {
            if (floorOutlineAlpha >= 0.0f)
                floorOutlineAlpha += 0.01f;
            if (floorOutlineAlpha >= 1.0f)
                floorOutlineAlpha = 0.0f;
            glm::vec4 color{ 1.0f, 0.28f, 0.26f, 0.0f };
            color.w = floorOutlineAlpha;

            glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // only draw according to stencil buffer

            glDisable(GL_CULL_FACE); // disable because floor has no Z dimension, the underside IS the BACK_FACE
            //floorRO.model[0] = Global::getModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(26.0f, 26.0f, 2.0f));
            floorRO.ssbo[5]->update(Global::camera.getViewProjectionMatrix() * glm::scale(floorRO.model[0], glm::vec3(1.05f, 1.05f, 0.0f))); // scale model by 5% for outline
            floorRO.ssbo[5]->upload();
            singleColorssboLayoutFloor.updateSubset(color);
            renderer.drawSingleColor(floorRO);
            glEnable(GL_CULL_FACE);

            // De-init Stencil Buffer
            glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test again
        }

        //std::println("End Renderpass time: {}ms", (static_cast<float>(glfwGetTime()) - loadTime) * 1000);

        // Draw frustum - toggle with K
        if (Global::frustumVisible) {
            //renderer.drawFrustum(cubeMesh, cameraDirLight.getViewProjectionMatrix());
            renderer.drawFrustum(cubeMesh, cameraSpotLight.getViewProjectionMatrix());
            //renderer.drawFrustum(cubeMesh, Global::cameraFlashLight.getViewProjectionMatrix());
        }

        // Draw debug quad - toggle with Q
        if (Global::debugQuadVisible) {
            // Set texture sampler2D binding in Shader itself + set orthographic in function + set corresponding Camera below
            renderer.drawDebugQuad(quadMesh, Global::cameraFlashLight);
        }

        if (!Global::paused) { // toggle with P
            glfwSwapBuffers(window);
        }

        glfwPollEvents();
    }
    Global::glCheckError();
    glfwTerminate();
    Global::glClearError(); // TODO glfwTerminate() produce errors
    return 0;
}