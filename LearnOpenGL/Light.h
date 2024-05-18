#pragma once

#include "Global.h"
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

// sendToShader, updatePosition and updateDirection functions transform World Space to View Space!

class Light {
public:
    const bool getOn() const { return m_on; };
    void setOn(bool x) { m_on = x; };
    const glm::vec3 getPosition() const { return m_position; };
    void setPosition(float x, float y, float z) { m_position.x = x, m_position.y = y, m_position.z = z; };
    const glm::vec3 getDirection() const { return m_direction; };
    void setDirection(float x, float y, float z) { m_direction.x = x, m_direction.y = y, m_direction.z = z; };
    const glm::vec3 getColor() const { return m_color; };
    void setColor(float x, float y, float z) { m_color.x = x, m_color.y = y, m_color.z = z; };
    const float getStrength() const { return m_strength; };
    void setStrength(float x) { m_strength = x; };
    const int getDepthMap() const { return m_depthMap; };
    void setDepthMap(int x) { m_depthMap = x; };

protected:
    Light() {};

    //lightType m_type{ lightType::undefined };
    bool m_on{ true };
    glm::vec3 m_position{};                             // World Space - not used for DirectionalLight
    glm::vec3 m_direction{};                            // World Space - not used for SpotLight
    glm::vec3 m_color{ 1.0f, 1.0f, 1.0f };              // Diffuse color
    float m_strength{ 1.0f };                           // Overall strength
    int m_depthMap{};                                   // sampler2D
};

// DirectionalLight //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DirectionalLight : public Light {
public:
    const float getAmbient() const { return m_ambient; };
    void setAmbient(float x) { m_ambient = x; };

    void sendToShader(Shader shader);
    void updateDirection(Shader shader);

protected:
    float m_ambient{ 0.3f };
};

// PointLight ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class PointLight : public Light {
public:
    void sendToShader(Shader shader);
    void updatePosition(Shader shader);
};

// SpotLight ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpotLight : public Light {
public:
    const float getConstant() const { return m_constant; };
    void setConstant(float x) { m_constant = x; };
    const float getLinear() const { return m_linear; };
    void setLinear(float x) { m_linear = x; };
    const float getQuadratic() const { return m_quadratic; };
    void setQuadratic(float x) { m_quadratic = x; };
    //const float getInnerCutOff() const { return m_innerCutOff; };
    void setInnerCutOff(float x) { m_innerCutOff = glm::cos(glm::radians(x)); };
    //const float getOuterCutOff() const { return m_outerCutOff; };
    void setOuterCutOff(float x) { m_outerCutOff = glm::cos(glm::radians(x)); };
    //const float getEpsilon() const { return m_innerCutOff - m_outerCutOff; };
    //void setEpsilon(float x) { m_epsilon = x; };

    virtual void sendToShader(Shader shader);
    virtual void updatePosition(Shader shader);
    virtual void updateDirection(Shader shader);
    virtual void updateColor(Shader shader);

protected:
    float m_constant{ 1.0f };         // Usually kept at 1.0f
    float m_linear{ 0.09f };          // Short distance intensity
    float m_quadratic{ 0.032f };      // Long distance intensity
    float m_innerCutOff{};            // Inner cone
    float m_outerCutOff{};            // Outer cone
    //float m_epsilon{};                // Gradually fade the light between inner and outer cone
};

// FlashLight ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FlashLight : public SpotLight {
public:
    const glm::vec3 getOrigin() const { return m_origin; };
    void setOrigin(glm::vec3 vec3) { m_origin = vec3; };
    const float getEmissionStrength() const { return m_emissionStrength; };
    void setEmissionStrength(float x) { m_emissionStrength = x; };

    void sendToShader(Shader shader) override; // Transform World Space to View Space
    void updatePosition(Shader shader) override;
    void updateDirection(Shader shader) override;

protected:
    glm::vec3 m_origin{ 0.0f, 0.0f, 0.0f };             // 0.0f, 0.0f, 0.0f == shines straight from the center/camera
    float m_emissionStrength{ 1.0f }; // Overall strength
};