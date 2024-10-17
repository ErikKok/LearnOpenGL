#pragma once

#include "Buffers.h"
#include "Camera.h"
#include "Data.h"
#include "Engine.h"
#include "Global.h"
#include "GlobalEntities.h"
#include "Light.h"
#include "Mesh.h"
#include "Model.h"
#include "Player.h"
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

#include <algorithm>
#include <array>
#include <memory> // for std::unique_ptr
#include <print>

int init(GLFWwindow* window)
{
    assert(sizeof(int) == sizeof(GLint) && "size of int and GL_INT is not equal");
    assert(sizeof(unsigned int) == sizeof(GLuint) && "size of unsigned int and GLuint is not equal");
    assert(sizeof(float) == sizeof(GLfloat) && "size of float and GL_FLOAT is not equal");
    assert(sizeof(unsigned char) == sizeof(GLubyte) && "size of int and GL_INT is not equal");

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, Engine::framebuffer_size_callback);
    glfwSetCursorEnterCallback(window, Engine::cursor_enter_callback);
    glfwSetCursorPosCallback(window, Engine::mouse_callback);
    glfwSetScrollCallback(window, Engine::scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, Engine::key_callback);

    std::println("Initialize GLAD");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::println("Failed to initialize GLAD");
        return -1;
    }

    std::println("Initialize OpenGL");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_FRAMEBUFFER_SRGB);

    std::println("Initialize Debug");
    int flags{};
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(G::glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    G::glCheckError();

    return 0;
}

int main()
{
    std::println("Configure GLFW");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    GLFWwindow* window = glfwCreateWindow(G::windowWidth, G::windowHeight, "LearnOpenGL", nullptr, nullptr);
    if (!window) {
        std::println("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    if (init(window) == -1) {
        std::println("Failed to initialize");
        glfwTerminate();
        return -1;
    }

    {
        std::println("Start OpenGL Scope");

        //Global::cheap2Copy();
        Renderer renderer;
        renderer.initStencilBuffer();
        renderer.createShaderSingleColor("Shaders\\singleColor.shader");
        renderer.createShaderSkybox("Shaders\\skybox.shader");
        renderer.createShaderFrustum("Shaders\\frustum.shader");
        renderer.createShaderDebugQuad("Shaders\\debugQuad.shader");
        renderer.createShaderDepthMap("Shaders\\depthMap.shader");
        //renderer.isRendererComplete();

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
        PointLight::pointLights[1].setStrength(2.4f);
        PointLight::pointLights[1].sendToShader(multiLight);
        PointLight::pointLights[1].sendToShader(multiLightNormalMapping);

        PointLight::pointLights.emplace_back(PointLight());
        PointLight::pointLights[2].setPosition({ -4.0f,  2.0f,  12.0f });
        PointLight::pointLights[2].setColor({ 0.0f, 1.0f, 0.0f });
        PointLight::pointLights[2].setStrength(1.0f);
        PointLight::pointLights[2].sendToShader(multiLight);
        PointLight::pointLights[2].sendToShader(multiLightNormalMapping);

        PointLight::pointLights.emplace_back(PointLight());
        PointLight::pointLights[3].setPosition({ 15.0f,  1.2f,  -3.0f });
        PointLight::pointLights[3].setColor({ 1.0f, 0.0f, 0.0f });
        PointLight::pointLights[3].setStrength(4.0f);
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

        //Texture depthMapDirLight(textureType::depthMap, 4096, 4096);
        renderer.m_FBO.emplace_back(std::make_unique<FrameBuffer>(4096, 4096));

        OrthographicCamera cameraDirLight(sun.getDirection(), -20.0f, 20.0f, -20.0f, 20.0f, -15.0f, 35.0f);

        // SpotLight0 | [0] == flashlight
        glm::vec3 flashLightOffset{ 0.4f, -0.1f, -0.1f };
        SpotLight::spotLights.emplace_back(SpotLight());
        SpotLight::spotLights[0].setOn(false);
        SpotLight::spotLights[0].setPosition(GE::camera.getPosition() + flashLightOffset);
        SpotLight::spotLights[0].setDirection({ 0.0f, 0.0f, -1.0f });
        SpotLight::spotLights[0].setColor({ 1.0f, 1.0f, 1.0f });
        SpotLight::spotLights[0].setStrength(5.5f); // waarom zo zwak resultaat? Omdat het bereik te ver of juist te kort is?
        SpotLight::spotLights[0].setDepthMap(5);
        SpotLight::spotLights[0].setConstant(1.0f);
        SpotLight::spotLights[0].setLinear(0.045f);
        SpotLight::spotLights[0].setQuadratic(0.0075f);
        SpotLight::spotLights[0].setInnerCutOff(8.5f);
        SpotLight::spotLights[0].setOuterCutOff(12.5f);
        SpotLight::spotLights[0].setEmissionStrength(1.0f);
        SpotLight::spotLights[0].sendToShader(multiLight);
        SpotLight::spotLights[0].sendToShader(multiLightNormalMapping);

        renderer.m_FBO.emplace_back(std::make_unique<FrameBuffer>(512, 512));

        SpotLight::spotLights[0].setCamera(Camera(renderer.m_FBO[1]->getTexture()->getAspectRatio(), G::cameraInitialPosition));
        SpotLight::spotLights[0].getCamera()->setFov(25.0f);
        SpotLight::spotLights[0].getCamera()->setNearPlane(0.1f);
        SpotLight::spotLights[0].getCamera()->setFarPlane(25.0f);

        // SpotLight1
        SpotLight::spotLights.emplace_back(SpotLight());
        SpotLight::spotLights[1].setPosition({ 0.0f, 20.0f, 0.0f });
        SpotLight::spotLights[1].setDirection({ 0.0f, -1.0f, 0.0f });
        SpotLight::spotLights[1].setColor({ 1.0f, 1.0f, 1.0f });
        SpotLight::spotLights[1].setStrength(1.2f);
        SpotLight::spotLights[1].setDepthMap(6);
        SpotLight::spotLights[1].setConstant(1.0f);
        SpotLight::spotLights[1].setLinear(0.014f);
        SpotLight::spotLights[1].setQuadratic(0.07f);
        SpotLight::spotLights[1].setInnerCutOff(36.0f);
        SpotLight::spotLights[1].setOuterCutOff(48.0f);
        SpotLight::spotLights[1].setEmissionStrength(0.0f);
        SpotLight::spotLights[1].sendToShader(multiLight);
        SpotLight::spotLights[1].sendToShader(multiLightNormalMapping);

        renderer.m_FBO.emplace_back(std::make_unique<FrameBuffer>(1024, 1024));

        SpotLight::spotLights[1].setCamera(Camera(renderer.m_FBO[2]->getTexture()->getAspectRatio(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        SpotLight::spotLights[1].getCamera()->setFov((36.0f + 48.0f) * 1.15f);
        SpotLight::spotLights[1].getCamera()->setNearPlane(0.1f);
        SpotLight::spotLights[1].getCamera()->setFarPlane(10.0f);

        /////////////////////////////////////
        ////// Quad /////////////////////////
        std::println("CREATE Quad");///////

        Mesh quadMesh(Data::quad, Data::quadIndices);

        /////////////////////////////////////
        ////// Cubes ////////////////////////
        std::println("CREATE Cubes");////////

        Mesh cubeMesh(Data::cube, Data::cubeIndices);

        Material cubeMaterial{
            .shader{ &multiLight },
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

        cubeRO.addSSBO(+SSBO::dirLightMVP, sizeof(glm::mat4), SSBO::dirLightMVP);
        cubeRO.addSSBO(+SSBO::spotLight0MVP, sizeof(glm::mat4), SSBO::spotLight0MVP);
        cubeRO.addSSBO(+SSBO::spotLight1MVP, sizeof(glm::mat4), SSBO::spotLight1MVP);
        cubeRO.addSSBO(+SSBO::normalMatrix, sizeof(glm::mat4));
        cubeRO.addSSBO(+SSBO::modelViewMatrix, sizeof(glm::mat4));
        cubeRO.addSSBO(+SSBO::MVP, sizeof(glm::mat4), SSBO::MVP);

        renderer.m_renderVector.emplace_back(&cubeRO);

        /////////////////////////////////////
        ////// Floor ////////////////////////
        std::println("CREATE Floor");////////

        Mesh floorMesh(Data::floor, Data::floorIndices);

        Material floorMaterial{
            .shader{ &multiLight },
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
        floorRO.transform[0] = G::calculateModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(25.0f, 25.0f, 1.0f));
        floorRO.drawOutline = true;

        floorRO.addSSBO(+SSBO::dirLightMVP, sizeof(glm::mat4), SSBO::dirLightMVP);
        floorRO.addSSBO(+SSBO::spotLight0MVP, sizeof(glm::mat4), SSBO::spotLight0MVP);
        floorRO.addSSBO(+SSBO::spotLight1MVP, sizeof(glm::mat4), SSBO::spotLight1MVP);
        floorRO.addSSBO(+SSBO::normalMatrix, sizeof(glm::mat4));
        floorRO.addSSBO(+SSBO::modelViewMatrix, sizeof(glm::mat4));
        floorRO.addSSBO(+SSBO::MVP, sizeof(glm::mat4), SSBO::MVP);
        floorRO.addSSBO(+SSBO::singleColor, sizeof(glm::vec4), SSBO::singleColor);
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
        //Remco/cubeandavatar.obj

        Material modelMaterial{
            .shader{ &multiLightNormalMapping },
            .diffuse1{ 0 },
            .specular1{ 0 },
            .normal1{ 7 },
            .shininess{ 666.0f },
            .emissionTexture{ 0 },
            .emissionStrength{ 0.0f },
            .lightEmissionMap{ 0 },
            .lightEmissionTexture{ 0 },
        };

        RenderObject modelRO{ nullptr, &modelMaterial };
        modelRO.model = std::make_unique<Model>("Models/Backpack/backpack.obj");
        modelRO.transform[0] = G::calculateModelMatrix(glm::vec3(4.0f, 3.0f, 2.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        modelRO.addSSBO(+SSBO::dirLightMVP, sizeof(glm::mat4), SSBO::dirLightMVP);
        modelRO.addSSBO(+SSBO::spotLight0MVP, sizeof(glm::mat4), SSBO::spotLight0MVP);
        modelRO.addSSBO(+SSBO::spotLight1MVP, sizeof(glm::mat4), SSBO::spotLight1MVP);
        modelRO.addSSBO(+SSBO::normalMatrix, sizeof(glm::mat4));
        modelRO.addSSBO(+SSBO::modelViewMatrix, sizeof(glm::mat4));
        modelRO.addSSBO(+SSBO::MVP, sizeof(glm::mat4), SSBO::MVP);

        renderer.m_renderVector.emplace_back(&modelRO);

        // Lightcubes
        RenderObject lightCubeRO{ &cubeMesh, {}, 5 };
        lightCubeRO.castsShadow = false;
        lightCubeRO.drawAsSingleColor = true;
        lightCubeRO.addSSBO(+SSBO::MVP, sizeof(glm::mat4), SSBO::MVP);
        lightCubeRO.addSSBO(+SSBO::singleColor, sizeof(glm::vec4), SSBO::singleColor);

        const std::vector<glm::vec4> lightCubeColors = {
        glm::vec4(PointLight::pointLights[0].getColor(), 1.0f), // magenta
        glm::vec4(PointLight::pointLights[1].getColor(), 1.0f), // white
        glm::vec4(PointLight::pointLights[2].getColor(), 1.0f), // green
        glm::vec4(PointLight::pointLights[3].getColor(), 1.0f), // blue
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), // draaiende lightCube, wordt later nog geupload
        };

        lightCubeRO.ssbo[1]->updateFully(lightCubeColors, false); // false, want in de loop wordt dit pas geupload, nadat element 4 is gewijzigd
        renderer.m_renderVector.emplace_back(&lightCubeRO);

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
        /* 07 */ Texture normalUpTexture("Textures\\normal_up.jpg", false); //0x807fffff); // TODO single color convertToLinearSpace implemented, but not tested!
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
        /* 02 */ renderer.m_FBO[0]->getTexture()->bind(2); // dirLight
        /* 03 */ flashLightTexture.bind(3);
        /* 04 */ floorTexture.bind(4);
        /* 05 */ renderer.m_FBO[1]->getTexture()->bind(5); // spotLight[0]
        /* 06 */ renderer.m_FBO[2]->getTexture()->bind(6); // spotLight[1]
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

        //GE::player.initDirection();

        G::getInformation();
        G::glCheckError();
        std::println("START Renderloop *******************************");
        std::println("************************************************");

        while (!glfwWindowShouldClose(window)) {
            Engine::ticksLoop++;
            Engine::perFrameTimeLogic();

            //std::println("deltaTime: {}ms", G::deltaTime * 1000);
            std::println("Position: {}, {}, {}", GE::camera.getPosition().x, GE::camera.getPosition().y, GE::camera.getPosition().z);
            //std::println("Front: {}, {}, {}", GE::camera.getFront().x, GE::camera.getFront().y, GE::camera.getFront().z);
            std::println("ticksLoop: {}", Engine::ticksLoop);
            std::println("ticksPhysics: {}", Engine::ticksPhysics);
            
            /////////////////////////////////////////////////////////////////////////////////////
            // Start processInput ///////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////

            glfwPollEvents();

            //GE::player.calculateDirection();
            Engine::processInput(window); // initMovement()
            Engine::doPhysics();
            Engine::doExtrapolationStep();

            // Teleporter (green light)
            // TODO disable extrapolation for an object on a frame where it's teleported.
            if (GE::camera.getPosition().x > -4.5f && GE::camera.getPosition().x < -3.5f &&
                GE::camera.getPosition().y >  1.5f && GE::camera.getPosition().y <  2.5f &&
                GE::camera.getPosition().z > 11.5f && GE::camera.getPosition().z < 12.5f ) {

                GE::camera.setYawPitch(-90.0f, 0.0f);
                GE::camera.setPosition(glm::vec3(0.0f, 8.0f, 30.0f));
            }

            if (!G::isFlashLightOnUpdated) {
                SpotLight::spotLights[0].toggle(multiLight, multiLightNormalMapping);
                G::isFlashLightOnUpdated = true;
            }

            GE::camera.calculateViewMatrix();

            /////////////////////////////////////////////////////////////////////////////////////
            // Start UpdateGame /////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////

            // Lights
            if (SpotLight::spotLights[0].getOn())
                G::applyCameraOffset(SpotLight::spotLights[0].getCamera(), flashLightOffset.x, flashLightOffset.y, flashLightOffset.z); // ExtrapolationStep needed for applyCameraOffset() -> setPosition() -> calculateViewMatrix()

            // ExtrapolationStep was needed for camera and flashlight, time to disable it
            Engine::isExtrapolationStep = false;

            sun.updateDirectionInViewSpace(multiLight);
            sun.updateDirectionInViewSpace(multiLightNormalMapping);

            for (auto& pointLight : PointLight::pointLights) {
                pointLight.updatePositionInViewSpace(multiLight);
                pointLight.updatePositionInViewSpace(multiLightNormalMapping);
            }

            SpotLight::spotLights[1].updateDirectionInViewSpace(multiLight);
            SpotLight::spotLights[1].updateDirectionInViewSpace(multiLightNormalMapping);
            //SpotLight::spotLights[1].setPosition({ 3.0f * static_cast<float>(sin(glfwGetTime())), 6.5f, static_cast<float>(4.5f * cos(glfwGetTime())) }); // cirkel draaien
            SpotLight::spotLights[1].setPosition(Engine::follow(SpotLight::spotLights[1].getPosition(), GE::camera.getPosition()));
            if (Engine::isEqual(SpotLight::spotLights[1].getPosition(), GE::camera.getPosition()))
                SpotLight::spotLights[1].setPosition({ 0.0f, 20.0f, 0.0f });
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

                cubeRO.transform[i] = glm::translate(glm::mat4(1.0f), Data::cubePositions[i]);
                if (i == 2 || i == 5 || i == 8) {
                    float angle = 25.0f + (15 * i);
                    cubeRO.transform[i] = glm::rotate(cubeRO.transform[i], (float)glfwGetTime() * glm::radians(100.0f) * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
                }
                if (i == 3) { // wall
                    cubeRO.transform[i] = glm::translate(cubeRO.transform[i], glm::vec3(0.0f, 0.0f, -3.0f));
                    cubeRO.transform[i] = glm::scale(cubeRO.transform[i], glm::vec3(20.0f, 20.0f, 1.0f));
                }
                //if (i == 9) { // floor
                //    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
                //    model = glm::scale(model, glm::vec3(20.0, 1.0, 20.0));
                //}

                cubeRO.transform[i] = glm::scale(cubeRO.transform[i], glm::vec3(0.5f, 0.5f, 0.5f));
            }

            // Calculate SSBO's, and upload them to their buffers
            for (auto i = 0; i < std::ssize(cubeRO.transform); i++) {
                G::modelViewMatrixTemp = GE::camera.getViewMatrix() * cubeRO.transform[i];
                cubeRO.ssbo[+SSBO::dirLightMVP]->updateSubset(cameraDirLight.getViewProjectionMatrix() * cubeRO.transform[i], i, false);
                cubeRO.ssbo[+SSBO::spotLight0MVP]->updateSubset(SpotLight::spotLights[0].getCamera()->getViewProjectionMatrix() * cubeRO.transform[i], i, false);
                cubeRO.ssbo[+SSBO::spotLight1MVP]->updateSubset(SpotLight::spotLights[1].getCamera()->getViewProjectionMatrix() * cubeRO.transform[i], i, false);
                cubeRO.ssbo[+SSBO::normalMatrix]->updateSubset(glm::transpose(glm::inverse(G::modelViewMatrixTemp)), i, false);
                cubeRO.ssbo[+SSBO::modelViewMatrix]->updateSubset(G::modelViewMatrixTemp, i, false);
                cubeRO.ssbo[+SSBO::MVP]->updateSubset(GE::camera.getProjectionMatrix() * G::modelViewMatrixTemp, i, false);
            }
            for (auto i = 0; i < std::ssize(cubeRO.ssbo); i++) {
                cubeRO.ssbo[i]->uploadFully();
            }

            // Floor
            for (auto i = 0; i < std::ssize(floorRO.transform); i++) {
                G::modelViewMatrixTemp = GE::camera.getViewMatrix() * floorRO.transform[i];
                floorRO.ssbo[+SSBO::dirLightMVP]->updateSubset(cameraDirLight.getViewProjectionMatrix() * floorRO.transform[i], i, false);
                floorRO.ssbo[+SSBO::spotLight0MVP]->updateSubset(SpotLight::spotLights[0].getCamera()->getViewProjectionMatrix() * floorRO.transform[i], i, false);
                floorRO.ssbo[+SSBO::spotLight1MVP]->updateSubset(SpotLight::spotLights[1].getCamera()->getViewProjectionMatrix() * floorRO.transform[i], i, false);
                floorRO.ssbo[+SSBO::normalMatrix]->updateSubset(glm::transpose(glm::inverse(G::modelViewMatrixTemp)), i, false);
                floorRO.ssbo[+SSBO::modelViewMatrix]->updateSubset(G::modelViewMatrixTemp, i, false);
                floorRO.ssbo[+SSBO::MVP]->updateSubset(GE::camera.getProjectionMatrix() * G::modelViewMatrixTemp, i, false);
            }
            for (auto i = 0; i < std::ssize(floorRO.ssbo); i++) {
                floorRO.ssbo[i]->uploadFully();
            }

            // Model
            for (auto i = 0; i < std::ssize(modelRO.transform); i++) {
                G::modelViewMatrixTemp = GE::camera.getViewMatrix() * modelRO.transform[i];
                modelRO.ssbo[+SSBO::dirLightMVP]->updateSubset(cameraDirLight.getViewProjectionMatrix() * modelRO.transform[i], i, false);
                modelRO.ssbo[+SSBO::spotLight0MVP]->updateSubset(SpotLight::spotLights[0].getCamera()->getViewProjectionMatrix() * modelRO.transform[i], i, false);
                modelRO.ssbo[+SSBO::spotLight1MVP]->updateSubset(SpotLight::spotLights[1].getCamera()->getViewProjectionMatrix() * modelRO.transform[i], i, false);
                modelRO.ssbo[+SSBO::normalMatrix]->updateSubset(glm::transpose(glm::inverse(G::modelViewMatrixTemp)), i, false);
                modelRO.ssbo[+SSBO::modelViewMatrix]->updateSubset(G::modelViewMatrixTemp, i, false);
                modelRO.ssbo[+SSBO::MVP]->updateSubset(GE::camera.getProjectionMatrix() * G::modelViewMatrixTemp, i, false);
            }
            for (auto i = 0; i < std::ssize(modelRO.ssbo); i++) {
                modelRO.ssbo[i]->uploadFully();
            }

            // LightCube
            // pointlights - 4 vaste LightCubes
            for (auto i = 0; i < getPointLightCount(); i++) {
                assert(getPointLightCount() <= lightCubeRO.instances && "Loop will create more instances then ssbo can hold");
                lightCubeRO.transform[i] = G::calculateModelMatrix(glm::vec3(PointLight::pointLights[i].getPosition()), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)); // you could move this to line below
                lightCubeRO.ssbo[0]->updateSubset(GE::camera.getViewProjectionMatrix() * lightCubeRO.transform[i], i, false);
            }

            // #5, element 4, de draaiende lightcube
            lightCubeRO.transform[4] = G::calculateModelMatrix(SpotLight::spotLights[1].getPosition(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)); // you could move this to line below
            lightCubeRO.ssbo[0]->updateSubset(GE::camera.getViewProjectionMatrix() * lightCubeRO.transform[4], 4, false);
            lightCubeRO.ssbo[0]->uploadFully();

            lightCubeRO.ssbo[1]->updateSubset(glm::vec4(SpotLight::spotLights[1].getColor(), 1.0f), 4, false);
            lightCubeRO.ssbo[1]->uploadFully();

            // Collision test //////////////////////////////////////////////////////////////////

            AABB wall{
                glm::vec3(10.0f,  10.0f, -2.5f), // m_vecMax
                glm::vec3(-10.0f, -10.0f, -3.5f)  // m_vecMin
            };

            if (Engine::AABBtoAABB(wall, GE::player.getTAABB()))
                std::println("COLLISION!!!");

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
            if (G::frustumVisible > 0) {
                switch (G::frustumVisible)
                {
                case 1:
                    renderer.drawFrustum(cubeMesh, cameraDirLight.getViewProjectionMatrix());
                    break;
                case 2:
                    renderer.drawFrustum(cubeMesh, SpotLight::spotLights[0].getCamera()->getViewProjectionMatrix());
                    break;
                case 3:
                    renderer.drawFrustum(cubeMesh, SpotLight::spotLights[1].getCamera()->getViewProjectionMatrix());
                    break;
                }
            }

            // Draw debug quad - toggle with Q
            if (G::debugQuadVisible > 0) {
                switch (G::debugQuadVisible)
                {
                case 1:
                    renderer.drawDebugQuad(quadMesh, cameraDirLight);
                    renderer.getShaderDebugQuad()->setInt("quadTexture", 2);
                    break;
                case 2:
                    renderer.drawDebugQuad(quadMesh, *SpotLight::spotLights[0].getCamera());
                    renderer.getShaderDebugQuad()->setInt("quadTexture", 5);
                    break;
                case 3:
                    renderer.drawDebugQuad(quadMesh, *SpotLight::spotLights[1].getCamera());
                    renderer.getShaderDebugQuad()->setInt("quadTexture", 6);
                    break;
                }
            }

            if (!G::paused) { // toggle with P
                glfwSwapBuffers(window);
            }

            // Limit frame rate
            //while (static_cast<float>(glfwGetTime()) - G::timestampLastFrame < 0.0333f) { // 0.00695f
            //    double calculate = 3.14 * 3.14 * 3.14;
            //    calculate *= 3.14;
            //}
                
        }
        G::glCheckError();
        std::println("Shutting down OpenGL scope");
    }
    glfwTerminate();
    system("pause");
    return 0;
}