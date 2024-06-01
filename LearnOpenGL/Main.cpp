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
    Global::cheap2Copy();
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

    glm::vec3 pointLightColors[] = {
    glm::vec3(1.0f, 1.0f, 1.0f), // red
    glm::vec3(0.0f, 1.0f, 0.0f), // green
    glm::vec3(0.0f, 0.0f, 1.0f), // blue
    glm::vec3(1.0f, 0.0f, 0.0f), // white
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
    sun.setColor(1.0f, 0.0f, 0.095f);
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
    spotLight.setStrength(2.2f);
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
    //flashLight.setPosition(0.0f, 1.5f, 15.0f); // TODO light position == camera position == needs to sync, or delete 1
    Global::cameraFlashLightPosition = { 0.0f, 1.5f, 15.0f }; // TODO light position == camera position == needs to sync, or delete 1
    //flashLight.setDirection(0.0f, -1.0f, 0.0f);
    flashLight.setColor(1.0f, 1.0f, 1.0f);
    flashLight.setStrength(5.5f); // waarom zo zwak resultaat?
    flashLight.setDepthMap(6);
    flashLight.setConstant(1.0f);
    flashLight.setLinear(0.045f);
    flashLight.setQuadratic(0.0075f);
    flashLight.setInnerCutOff(8.5f);
    flashLight.setOuterCutOff(12.5f);
    flashLight.setEmissionStrength(0.8f);
    flashLight.setOffset(0.4f, -0.5f, -0.3f);
    flashLight.sendToShader(multiLight); 
    flashLight.sendToShader(multiLightNormalMapping);

    Camera flashLightCamera(1.0f, glm::vec3(0.0f, 1.5f, 15.0f) + glm::vec3(0.4f, -0.5f, -0.3f)); // TODO get aspectratio from depthmap texture

    // FlashLight depthMap
    Texture depthMapFlashLight(textureType::depthMap, 1920, 1080);
    FrameBuffer depthMapFlashLightFBO(depthMapFlashLight);
    Global::cameraFlashLight.setFov(60.0f);
    Global::cameraFlashLight.setNearPlane(0.1f);
    Global::cameraFlashLight.setFarPlane(400.0f);

    /////////////////////////////////////
    ////// Quad /////////////////////////
    //std::println("CREATE Quad");/////////

    Mesh quadMesh(Data::quad, Data::quadIndices);

    /////////////////////////////////////
    ////// Cubes ////////////////////////
    std::println("CREATE Cubes");////////

    Mesh cubeMesh(Data::cube, Data::cubeIndices);

    Material cubeMaterial{
        .shader{ multiLight },
        .diffuse1{ 8 },
        .specular1{ 9 },
        .normal1{ 0 },
        .emission{ 10 },
        .emissionStrength{ 0.5f },
        .shininess{ 32.0f },
        .flashLightEmissionMap{ 11 },
        .flashLightEmissionTexture{ 12 },
    };

    /////////////////////////////////////
    ////// Floor ////////////////////////
    std::println("CREATE Floor");////////

    Mesh floorMesh(Data::floor, Data::floorIndices);

    float floorOutlineAlpha{ 0.0f };

    Material floorMaterial{
        .shader{ multiLight },
        .diffuse1{ 4 },
        .specular1{ 0 },
        .normal1{ 0 },
        .emission{ 0 },
        .emissionStrength{ 0.0f },
        .shininess{ 8.0f },
        .flashLightEmissionMap{ 0 },
        .flashLightEmissionTexture{ 12 },
    };

    ////////////////////////////////////
    ////// Models //////////////////////
    std::println("LOAD Model");/////////

    float loadTime{ static_cast<float>(glfwGetTime()) };

    //Shader ourModelShader("Shaders\\multiLight.shader");
    Model ourModel("Models/Backpack/backpack.obj");
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
        .normal1{ 0 },
        .emission{ 0 },
        .emissionStrength{ 0.0f },
        .shininess{ 666.0f },
        .flashLightEmissionMap{ 0 },
        .flashLightEmissionTexture{ 0 },
    };

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
    /* 07 */
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
    /* 07 */
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

        // Voor flashLight:
        //Global::camera.getPostion;
        //Global::camera.getFront;
        //Global::camera.getUp;
        //Global::camera.getYaw;
        //Global::camera.getPitch;
        //Global::camera.getFov;
        //dan set Position met offset en de rest;
        //dan Global::cameraFlashLight.calculateViewMatrix(); (en daarmee de ViewProjectionMatrix);

        //als Fov is veranderd, dan ook calculateProjectionMatrix doen 

        /////////////////////////////////////
        ////// Lights ///////////////////////
        /////////////////////////////////////

        // Transform Direction dirLight to current View Space
        sun.updateDirectionInViewSpace(multiLight);

        multiLight.useShader();
        multiLight.setVec3("pointLights[0].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[0], 1.0)));  // red
        multiLight.setVec3("pointLights[1].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[1], 1.0)));  // green
        multiLight.setVec3("pointLights[2].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[2], 1.0)));  // blue
        multiLight.setVec3("pointLights[3].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[3], 1.0)));  // white

        // Transform Spotlight direction to View Space
        spotLight.updateDirectionInViewSpace(multiLight);
        // Calculate Spotlight position and transform to View Space
        spotLight.setPosition(3.0f * static_cast<float>(sin(glfwGetTime())), 6.5f, static_cast<float>(4.5f * cos(glfwGetTime())));
        spotLight.updatePositionInViewSpace(multiLight);
        // Calculate Spotlight color
        spotLight.setColor(static_cast<float>(sin(glfwGetTime() * 0.25f)), static_cast<float>(sin(glfwGetTime() * 0.50f)), static_cast<float>(sin(glfwGetTime() * 0.75f)));
        spotLight.updateColor(multiLight);

        /////

        // Transform Direction dirLight to current View Space
        sun.updateDirectionInViewSpace(multiLightNormalMapping);

        multiLightNormalMapping.useShader();
        multiLightNormalMapping.setVec3("pointLights[0].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[0], 1.0)));  // red
        multiLightNormalMapping.setVec3("pointLights[1].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[1], 1.0)));  // green
        multiLightNormalMapping.setVec3("pointLights[2].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[2], 1.0)));  // blue
        multiLightNormalMapping.setVec3("pointLights[3].position", glm::vec3(Global::camera.getViewMatrix() * glm::vec4(pointLightPositions[3], 1.0)));  // white

        // Transform Spotlight direction to View Space
        spotLight.updateDirectionInViewSpace(multiLightNormalMapping);
        // Calculate Spotlight position and transform to View Space
        spotLight.setPosition(3.0f * static_cast<float>(sin(glfwGetTime())), 6.5f, static_cast<float>(4.5f * cos(glfwGetTime())));
        spotLight.updatePositionInViewSpace(multiLightNormalMapping);
        // Calculate Spotlight color
        spotLight.setColor(static_cast<float>(sin(glfwGetTime() * 0.25f)), static_cast<float>(sin(glfwGetTime() * 0.50f)), static_cast<float>(sin(glfwGetTime() * 0.75f)));
        spotLight.updateColor(multiLightNormalMapping);


        // TODO
        //glm::vec3 dir = flashLightCamera.m_front;
        //multiLightNormalMapping.setVec3("flashLight.direction", { dir.x, dir.y, -dir.z });
        //flashLight.updateDirection(multiLightNormalMapping);

        /////////////////////////////////////////////////////////////////////////////////////
        // Start ShadowPass dirLight ////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        loadTime = static_cast<float>(glfwGetTime());

        renderer.setRenderPassActive(renderPassType::depthMapDirLight);

        depthMapDirLightFBO.startDepthMap(renderer.getShaderDepthMapDirLight());

        /////////////////////////////////////
        ////// Cubes ShadowPass dirLight ////
        /////////////////////////////////////

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
            //if (i == 9) { // floor
            //    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            //    model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
            //}
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            dirLightMVPMatrixSSBO.setVector(cameraDirLight.getViewProjectionMatrix() * model, i);
        }
        dirLightMVPMatrixSSBO.updateAndBind();

        renderer.draw(cubeMesh, cubeMaterial, std::size(Data::cubePositions));

        /////////////////////////////////////
        ////// Model ShadowPass dirLight ////
        /////////////////////////////////////

        model = Global::getModelMatrix(glm::vec3(4.0f, 3.0f, 2.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        dirLightMVPMatrixSSBO.setVectorAndUpdateAndBind(cameraDirLight.getViewProjectionMatrix() * model);

        ourModel.draw(modelMaterial, renderer);

        /////////////////////////////////////
        ////// Floor ShadowPass dirLight ////
        /////////////////////////////////////

        model = Global::getModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(25.0f, 25.0f, 2.0f)); // TODO deze waarde wordt met de 2e renderpass ook gebruikt, herbruiken dus
        dirLightMVPMatrixSSBO.setVectorAndUpdateAndBind(cameraDirLight.getViewProjectionMatrix() * model);

        renderer.draw(floorMesh, floorMaterial);

        /////////////////////////////////////////////////////////////////////////////////////
        // End ShadowPass dirLight //////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        depthMapDirLightFBO.stopDepthMap();
        //std::println("End ShadowPass dirLight time: {}ms", (static_cast<float>(glfwGetTime()) - loadTime) * 1000);

        /////////////////////////////////////////////////////////////////////////////////////
        // Start ShadowPass spotLight ///////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        loadTime = static_cast<float>(glfwGetTime());

        renderer.setRenderPassActive(renderPassType::depthMapSpotLight);

        depthMapSpotLightFBO.startDepthMap(renderer.getShaderDepthMapSpotLight());

        // TODO sync light position with camera classes
        glm::mat4 view = glm::lookAt(spotLight.getPosition(), spotLight.getPosition() + glm::vec3(0.0f, -spotLight.getPosition().y, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)); // cameraPos + glm::vec3(0.0f, -cameraPos.y, 0.0f) == glm::vec3(cameraPos.x, 0.0f, cameraPos.z)

        cameraSpotLight.setViewMatrix(view); // TODO

        /////////////////////////////////////
        ////// Cubes ShadowPass spotLight ///
        /////////////////////////////////////

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
            //if (i == 9) { // floor
            //    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            //    model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
            //}
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            spotLightMVPMatrixSSBO.setVector(cameraSpotLight.getViewProjectionMatrix() * model, i);
        }
        spotLightMVPMatrixSSBO.updateAndBind();

        renderer.draw(cubeMesh, cubeMaterial, std::size(Data::cubePositions));

        /////////////////////////////////////
        ////// Model ShadowPass spotLight ///
        /////////////////////////////////////

        model = Global::getModelMatrix(glm::vec3(4.0f, 3.0f, 2.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        spotLightMVPMatrixSSBO.setVectorAndUpdateAndBind(cameraSpotLight.getViewProjectionMatrix() * model);

        ourModel.draw(modelMaterial, renderer);

        /////////////////////////////////////
        ////// Floor ShadowPass spotLight ///
        /////////////////////////////////////

        model = Global::getModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(25.0f, 25.0f, 2.0f)); // TODO deze waarde wordt met de 2e renderpass ook gebruikt, herbruiken dus
        spotLightMVPMatrixSSBO.setVectorAndUpdateAndBind(cameraSpotLight.getViewProjectionMatrix() * model);

        renderer.draw(floorMesh, floorMaterial);

        /////////////////////////////////////////////////////////////////////////////////////
        // End ShadowPass spotLight /////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        depthMapSpotLightFBO.stopDepthMap();
        //std::println("End ShadowPass spotLight time: {}ms", (static_cast<float>(glfwGetTime()) - loadTime) * 1000);

        /////////////////////////////////////////////////////////////////////////////////////
        // Start ShadowPass flashLight //////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        loadTime = static_cast<float>(glfwGetTime());

        renderer.setRenderPassActive(renderPassType::depthMapFlashLight);

        depthMapFlashLightFBO.startDepthMap(renderer.getShaderDepthMapFlashLight());

        /////////////////////////////////////
        ////// Cubes ShadowPass flashLight //
        /////////////////////////////////////

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
            //if (i == 9) { // floor
            //    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            //    model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
            //}
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            flashLightMVPMatrixSSBO.setVector(Global::cameraFlashLight.getViewProjectionMatrix() * model, i);
        }
        flashLightMVPMatrixSSBO.updateAndBind();

        renderer.draw(cubeMesh, cubeMaterial, std::size(Data::cubePositions));

        /////////////////////////////////////
        ////// Model ShadowPass flashLight //
        /////////////////////////////////////

        model = Global::getModelMatrix(glm::vec3(4.0f, 3.0f, 2.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        flashLightMVPMatrixSSBO.setVectorAndUpdateAndBind(Global::cameraFlashLight.getViewProjectionMatrix() * model);

        ourModel.draw(modelMaterial, renderer);

        /////////////////////////////////////
        ////// Floor ShadowPass flashLight //
        /////////////////////////////////////

        model = Global::getModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(25.0f, 25.0f, 2.0f)); // TODO deze waarde wordt met de 2e renderpass ook gebruikt, herbruiken dus
        flashLightMVPMatrixSSBO.setVectorAndUpdateAndBind(Global::cameraFlashLight.getViewProjectionMatrix() * model);

        renderer.draw(floorMesh, floorMaterial);

        /////////////////////////////////////////////////////////////////////////////////////
        // End ShadowPass flashLight ////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        depthMapFlashLightFBO.stopDepthMap();
        //std::println("End ShadowPass spotLight time: {}ms", (static_cast<float>(glfwGetTime()) - loadTime) * 1000);

        /////////////////////////////////////////////////////////////////////////////////////
        // Render scene normal //////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////

        loadTime = static_cast<float>(glfwGetTime());

        renderer.setRenderPassActive(renderPassType::normal);

        glViewport(0, 0, Global::windowWidth, Global::windowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!Global::flashLightOnUpdated) {
            flashLight.toggle(multiLight, multiLightNormalMapping);
            Global::flashLightOnUpdated = true;
        }

        //Global::camera.fakeGravity(Global::deltaTime);

        /////////////////////////////////////
        ////// XYZ //////////////////////////
        /////////////////////////////////////

        //renderer.drawXYZ(MVPMatrixSSBO); // Non-DSA

        /////////////////////////////////////
        ////// LightCube ////////////////////
        /////////////////////////////////////

        MVPMatrixSSBO.setVectorAndUpdateAndBind(Global::camera.getProjectionMatrix() * Global::getModelViewMatrix(spotLight.getPosition(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)));

        renderer.drawSingleColor(cubeMesh, glm::vec4(spotLight.getColor(), 1.0f), 1);

        // pointlights - 4 vaste LightCubes
        for (int i = 0; i < std::size(pointLightPositions); i++) {
            assert(std::size(pointLightPositions) <= ArrayCountSSBO && "Loop will create more instances then ssbo can hold");
            MVPMatrixSSBO.setVector(Global::camera.getProjectionMatrix() * Global::getModelViewMatrix(glm::vec3(pointLightPositions[i]), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)), i);
        }
        MVPMatrixSSBO.updateAndBind();

        renderer.drawSingleColor(cubeMesh, { 1.0f, 0.0f, 1.0f, 1.0f }, 4); // TODO deze color uniform/parameter is geen array, dus kleur is zelfde voor alle cubes

        /////////////////////////////////////
        ////// Cubes ////////////////////////
        /////////////////////////////////////

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
                model = glm::scale(model, glm::vec3(20.0f, 20.0f, 1.0f));
            }
            //if (i == 9) { // floor
            //    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            //    model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
            //}
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

            modelViewMatrix = Global::camera.getViewMatrix() * model;
            NormalMatrixSSBO.setVector(glm::transpose(glm::inverse(modelViewMatrix)), i);
            ModelViewMatrixSSBO.setVector(modelViewMatrix, i);
            MVPMatrixSSBO.setVector(Global::camera.getProjectionMatrix() * modelViewMatrix, i);
            dirLightMVPMatrixSSBO.setVector(cameraDirLight.getViewProjectionMatrix() * model, i);
            spotLightMVPMatrixSSBO.setVector(cameraSpotLight.getViewProjectionMatrix() * model, i);
            flashLightMVPMatrixSSBO.setVector(Global::cameraFlashLight.getViewProjectionMatrix() * model, i);
        }

        NormalMatrixSSBO.updateAndBind();
        ModelViewMatrixSSBO.updateAndBind();
        MVPMatrixSSBO.updateAndBind();
        dirLightMVPMatrixSSBO.updateAndBind();      // TODO is al eerder berekend...
        spotLightMVPMatrixSSBO.updateAndBind();     // TODO is al eerder berekend...
        flashLightMVPMatrixSSBO.updateAndBind();    // TODO is al eerder berekend...

        renderer.draw(cubeMesh, cubeMaterial, std::size(Data::cubePositions));

        /////////////////////////////////////
        ////// Model ////////////////////////
        /////////////////////////////////////

        model = Global::getModelMatrix(glm::vec3(4.0f, 3.0f, 2.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        modelViewMatrix = Global::camera.getViewMatrix() * model;
        NormalMatrixSSBO.setVectorAndUpdateAndBind(glm::transpose(glm::inverse(modelViewMatrix)));
        ModelViewMatrixSSBO.setVectorAndUpdateAndBind(modelViewMatrix);
        MVPMatrixSSBO.setVectorAndUpdateAndBind(Global::camera.getProjectionMatrix() * modelViewMatrix);
        dirLightMVPMatrixSSBO.setVectorAndUpdateAndBind(cameraDirLight.getViewProjectionMatrix() * model);
        spotLightMVPMatrixSSBO.setVectorAndUpdateAndBind(cameraSpotLight.getViewProjectionMatrix() * model);
        flashLightMVPMatrixSSBO.setVectorAndUpdateAndBind(Global::cameraFlashLight.getViewProjectionMatrix() * model);

        //multiLightNormalMapping.useShader();
        //multiLightNormalMapping.setMat4("model", model);
        //multiLightNormalMapping.setMat4("view", Global::camera.getViewMatrix());
        //multiLightNormalMapping.setMat4("projection", Global::camera.getProjectionMatrix());

        ourModel.draw(modelMaterial, renderer);

        /////////////////////////////////////
        ////// Floor ////////////////////////
        /////////////////////////////////////

        glStencilMask(0xFF); // enable writing to the stencil buffer

        model = Global::getModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(25.0f, 25.0f, 2.0f));
        modelViewMatrix = Global::camera.getViewMatrix() * model;
        NormalMatrixSSBO.setVectorAndUpdateAndBind(glm::transpose(glm::inverse(modelViewMatrix)));
        ModelViewMatrixSSBO.setVectorAndUpdateAndBind(modelViewMatrix);
        MVPMatrixSSBO.setVectorAndUpdateAndBind(Global::camera.getProjectionMatrix() * modelViewMatrix);
        dirLightMVPMatrixSSBO.setVectorAndUpdateAndBind(cameraDirLight.getViewProjectionMatrix() * model);
        spotLightMVPMatrixSSBO.setVectorAndUpdateAndBind(cameraSpotLight.getViewProjectionMatrix() * model);
        flashLightMVPMatrixSSBO.setVectorAndUpdateAndBind(Global::cameraFlashLight.getViewProjectionMatrix() * model);

        glDisable(GL_CULL_FACE); // disable because floor has no Z dimension, the underside IS the BACK_FACE
        renderer.draw(floorMesh, floorMaterial);
        glEnable(GL_CULL_FACE);

        glStencilMask(0x00); // disable writing to the stencil buffer

        /////////////////////////////////////
        ////// Skybox ///////////////////////
        /////////////////////////////////////
        
        renderer.drawSkybox(cubeMesh);

        /////////////////////////////////////
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

            modelViewMatrix = Global::getModelViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0, 0.0, 0.0), glm::vec3(26.0, 26.0, 2.0));
            MVPMatrixSSBO.setVectorAndUpdateAndBind(Global::camera.getProjectionMatrix() * modelViewMatrix);

            glDisable(GL_CULL_FACE); // disable because floor has no Z dimension, the underside IS the BACK_FACE
            renderer.drawSingleColor(floorMesh, color);
            glEnable(GL_CULL_FACE);

            // De-init Stencil Buffer
            glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test again
        }

        //std::println("End Renderpass time: {}ms", (static_cast<float>(glfwGetTime()) - loadTime) * 1000);

        // Draw frustum - toggle with K
        if (Global::frustumVisible) {
            renderer.drawFrustum(cubeMesh, cameraDirLight.getViewProjectionMatrix());
            renderer.drawFrustum(cubeMesh, cameraSpotLight.getViewProjectionMatrix());
            //renderer.drawFrustum(cubeMesh, Global::cameraFlashLight.getViewProjectionMatrix());
        }

        // Draw debug quad - toggle with Q
        if (Global::debugQuadVisible) {
            // Set texture sampler2D binding in Shader itself + set orthographic true in function for dirLight + set corresponding Camera below
            renderer.drawDebugQuad(quadMesh, cameraDirLight);
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