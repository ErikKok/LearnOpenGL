#pragma once

#include "Shader.h"

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <array>

//enum class lightType {
//    undefined,
//    directional,
//    point,
//    spot,
//    flash,
//    max_lightTypes,
//};
//
//constexpr std::array lightTypeName{ "undefined", "directional", "point", "spot", "flash" };
//static_assert(std::size(lightTypeName) == static_cast<int>(lightType::max_lightTypes));

class Light {
public:
    const bool getOn() const { return m_on; };
    void setOn(bool x) { m_on = x; };
    const glm::vec3 getPosition() const { return m_position; };
    void setPosition(glm::vec3 vec3) { m_position = vec3; };
    const glm::vec3 getDirection() const { return m_direction; };
    void setDirection(glm::vec3 vec3) { m_direction = vec3; };
    const glm::vec3 getColor() const { return m_color; };
    void setColor(glm::vec3 vec3) { m_color = vec3; };
    const float getStrength() const { return m_strength; };
    void setStrength(float x) { m_strength = x; };
    const int getDepthMap() const { return m_depthMap; };
    void setDepthMap(int x) { m_depthMap = x; };


protected:
    Light() {};
    //Light(glm::vec3 position)
    //    : m_position{ position } 
    //{};

    //lightType m_type{ lightType::undefined };
    bool m_on{ true };
    glm::vec3 m_position{};                             // View Space
    glm::vec3 m_direction{};                            // View Space - not used for DirectionalLight
    glm::vec3 m_color{ 1.0f, 1.0f, 1.0f };              // Diffuse color
    float m_strength{ 1.0f };                           // Overall strength
    int m_depthMap{};                                   // sampler2D
};

class DirectionalLight : public Light {
public:
    const float getAmbient() const { return m_ambient; };
    void setAmbient(float x) { m_ambient = x; };

    void sendToShader(Shader shader);

protected:
    float m_ambient{ 0.3f };
};

class SpotLight : public Light {
public:
    //SpotLight(glm::vec3 position)
    //    : Light{ position }
    //{};

    const float getConstant() const { return m_constant; };
    void setConstant(float x) { m_constant = x; };
    const float getLinear() const { return m_linear; };
    void setLinear(float x) { m_linear = x; };
    const float getQuadratic() const { return m_quadratic; };
    void setQuadratic(float x) { m_quadratic = x; };
    const float getOuterCutOff() const { return m_outerCutOff; };
    void setOuterCutOff(float x) { m_outerCutOff = x; };
    const float getEpsilon() const { return m_epsilon; };
    void setEpsilon(float x) { m_epsilon = x; };

protected:
    float m_constant{ 1.0f };         // Usually kept at 1.0f
    float m_linear{ 0.09f };          // Short distance intensity
    float m_quadratic{ 0.032f };      // Long distance intensity
    float m_outerCutOff{};            // Outer cone
    float m_epsilon{};                // Gradually fade the light between inner and outer cone
};

class FlashLight : public SpotLight {
public:
    //FlashLight(glm::vec3 position)
    //    : SpotLight{ position }
    //{};

    const glm::vec3 getOrigin() const { return m_origin; };
    void setOrigin(glm::vec3 vec3) { m_origin = vec3; };
    const float getEmissionStrength() const { return m_emissionStrength; };
    void setEmissionStrength(float x) { m_emissionStrength = x; };

protected:
    glm::vec3 m_origin{ 0.0f, 0.0f, 0.0f };             // 0.0f, 0.0f, 0.0f == shines straight from the center/camera
    float m_emissionStrength{ 1.0f }; // Overall strength
};