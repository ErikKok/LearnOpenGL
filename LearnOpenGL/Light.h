#pragma once

#include "Global.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light {
public:
    const bool getOn() const { return m_on; };
    void setOn(bool x) { m_on = x; };
    const glm::vec3 getPosition() const { return m_position; };
    void setPosition(glm::vec3 x) { m_position = x; };
    const glm::vec3 getDirection() const { return m_direction; };
    void setDirection(glm::vec3 x) { m_direction = x; };
    const glm::vec3 getColor() const { return m_color; };
    void setColor(glm::vec3 x) { m_color = x; };
    const float getStrength() const { return m_strength; };
    void setStrength(float x) { m_strength = x; };
    const int getDepthMap() const { return m_depthMap; };
    void setDepthMap(int x) { m_depthMap = x; };

protected:
    Light() {};

    bool m_on{ true };
    glm::vec3 m_position{};                     // World Space - not used for DirectionalLight
    glm::vec3 m_direction{};                    // World Space - not used for PointLight
    glm::vec3 m_color{ 1.0f, 1.0f, 1.0f };      // Diffuse color
    float m_strength{ 1.0f };                   // Overall strength
    int m_depthMap{};                           // sampler2D
};

// DirectionalLight //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DirectionalLight : public Light {
public:
    const float getAmbient() const { return m_ambient; };
    void setAmbient(float x) { m_ambient = x; };

    void sendToShader(const Shader& shader) const;
    void updateDirectionInViewSpace(const Shader& shader) const;

protected:
    float m_ambient{ 0.3f };
};

// PointLight ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class PointLight : public Light {
public:
    PointLight()
        : m_id{ m_countPointLight++ }
    {}

    #pragma warning( suppress : 4100 ) // TODO ugly way for derived class to use this constructor which does not increase m_id
    PointLight(bool increase_m_id)
    {}

    static inline int m_countPointLight{ 0 };
    static inline std::vector<PointLight> pointLights;

    void sendToShader(const Shader& shader) const;
    void updatePositionInViewSpace(const Shader& shader) const;
    const float getConstant() const { return m_constant; };
    void setConstant(float x) { m_constant = x; };
    const float getLinear() const { return m_linear; };
    void setLinear(float x) { m_linear = x; };
    const float getQuadratic() const { return m_quadratic; };
    void setQuadratic(float x) { m_quadratic = x; };

protected:
    int m_id{};                     // zero based
    float m_constant{ 1.0f };       // Usually kept at 1.0f
    float m_linear{ 0.09f };        // Short distance intensity
    float m_quadratic{ 0.032f };    // Long distance intensity
};

//inline const int getPointLightCount() { return static_cast<int>(std::ssize(PointLight::pointLights)); };
inline const int getPointLightCount() { return PointLight::m_countPointLight; };

// SpotLight ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpotLight : public PointLight {
public:
    SpotLight()
        : PointLight(false)
        , m_id{ m_countSpotLight++ }
    {}

    #pragma warning( suppress : 4100 ) // TODO ugly way for derived class to use this constructor which does not increase m_id
    SpotLight(bool increase_m_id)
        : PointLight(false)
    {}

    static inline int m_countSpotLight{ 0 };
    static inline std::vector<SpotLight> spotLights;

    //const float getInnerCutOff() const { return m_innerCutOff; }; // need to convert radians back to degrees
    void setInnerCutOff(float x) { m_innerCutOff = glm::cos(glm::radians(x)); };
    //const float getOuterCutOff() const { return m_outerCutOff; }; // need to convert radians back to degrees
    void setOuterCutOff(float x) { m_outerCutOff = glm::cos(glm::radians(x)); };

    virtual void sendToShader(const Shader& shader) const;
    virtual void updatePositionInViewSpace(const Shader& shader) const;
    virtual void updateDirectionInViewSpace(const Shader& shader) const;
    virtual void updateColor(const Shader& shader) const;
    void setEmissionStrength(float x) { m_emissionStrength = x; };
    void toggle(const Shader& shader, const Shader& shader2); // TODO werkt nu alleen met exact 2 shaders... kan met array en loop

protected:
    int m_id{};                         // zero based
    float m_innerCutOff{};              // Inner cone
    float m_outerCutOff{};              // Outer cone
    float m_emissionStrength{ 0.0f };   // Overall strength
};

inline const int getSpotLightCount() { return SpotLight::m_countSpotLight; };

// FlashLight ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//class FlashLight : public SpotLight {
//public:
//    FlashLight()
//        : SpotLight(false)
//    {}
//
//    //const glm::vec3 getOrigin() const { return m_origin; };
//    //void setOrigin(float x, float y, float z) { m_origin.x = x, m_origin.y = y, m_origin.z = z; };
//    //const float getEmissionStrength() const { return m_emissionStrength; };
//    //void setEmissionStrength(float x) { m_emissionStrength = x; };
//
//    void sendToShader(const Shader& shader) const override; // Transform World Space to View Space // TODO geen idee wat ik bedoel...
//    //void updatePositionInViewSpace(const Shader& shader) const override;
//    //void updateDirectionInViewSpace(const Shader& shader) const override;
//    void toggle(const Shader& shader, const Shader& shader2); // TODO werkt nu alleen met exact 2 shaders... kan met array en loop
//
//protected:
//    //glm::vec3 m_origin{ 0.0f, 0.0f, 0.0f }; // 0.0f, 0.0f, 0.0f == shines straight from the center/camera, offset for holding flashlight in right hand
//    float m_emissionStrength{ 1.0f }; // Overall strength
//};