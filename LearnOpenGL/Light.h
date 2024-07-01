#pragma once

#include "Global.h"
#include <memory> // for std::unique_ptr and std::make_unique
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light {
public:
    bool getOn() const { return m_on; };
    void setOn(bool x) { m_on = x; };
    const glm::vec3& getPosition() const { return m_position; };
    void setPosition(glm::vec3 x);
    const glm::vec3& getDirection() const { return m_direction; };
    void setDirection(glm::vec3 x) { m_direction = x; };
    const glm::vec3& getColor() const { return m_color; };
    void setColor(glm::vec3 x) { m_color = x; };
    float getStrength() const { return m_strength; };
    void setStrength(float x) { m_strength = x; };
    int getDepthMap() const { return m_depthMap; };
    void setDepthMap(int x) { m_depthMap = x; };
    void setCamera(Camera x) { m_camera = std::make_unique<Camera>(x); };
    template <class Self>
    auto&& getCamera(this Self&& self) {
        return std::forward<Self>(self).m_camera;
    }

protected:
    Light() {};

    bool m_on{ true };                          // If false then light is not caluted in Shader (use this instead of strength 0.0f)
    glm::vec3 m_position{};                     // World Space - not used for DirectionalLight
    glm::vec3 m_direction{};                    // World Space - not used for PointLight
    glm::vec3 m_color{ 1.0f, 1.0f, 1.0f };      // Diffuse color
    float m_strength{ 1.0f };                   // Overall strength
    int m_depthMap{};                           // sampler2D
    std::unique_ptr<Camera> m_camera{ nullptr };
};

// DirectionalLight //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DirectionalLight : public Light {
public:
    float getAmbient() const { return m_ambient; };
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
    float getConstant() const { return m_constant; };
    void setConstant(float x) { m_constant = x; };
    float getLinear() const { return m_linear; };
    void setLinear(float x) { m_linear = x; };
    float getQuadratic() const { return m_quadratic; };
    void setQuadratic(float x) { m_quadratic = x; };

protected:
    int m_id{};                     // zero based
    float m_constant{ 1.0f };       // Usually kept at 1.0f
    float m_linear{ 0.09f };        // Short distance intensity
    float m_quadratic{ 0.032f };    // Long distance intensity
};

inline int getPointLightCount() { return PointLight::m_countPointLight; };

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

    void sendToShader(const Shader& shader) const;
    void updatePositionInViewSpace(const Shader& shader) const;
    void updateDirectionInViewSpace(const Shader& shader) const;
    void updateColor(const Shader& shader) const;
    void setEmissionStrength(float x) { m_emissionStrength = x; };
    void toggle(const Shader& shader, const Shader& shader2); // TODO werkt nu alleen met exact 2 shaders... kan met array en loop

protected:
    int m_id{};                         // zero based
    float m_innerCutOff{};              // Inner cone
    float m_outerCutOff{};              // Outer cone
    float m_emissionStrength{ 0.0f };   // Overall strength
};

inline int getSpotLightCount() { return SpotLight::m_countSpotLight; };