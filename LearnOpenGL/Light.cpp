#pragma once

#include "Light.h"

// DirectionalLight //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DirectionalLight::sendToShader(Shader shader)
{
    //shader.setInt("dirLight.on", m_on);
    shader.setVec3("dirLight.direction", Global::view * glm::vec4(m_direction, 0.0f));
    shader.setVec3("dirLight.color", m_color);
    shader.setFloat("dirLight.strength", m_strength);
    shader.setFloat("dirLight.ambient", m_ambient);
    shader.setInt("dirLight.depthMap", m_depthMap);
}

void DirectionalLight::updateDirection(Shader shader)
{
    shader.setVec3("dirLight.direction", Global::view * glm::vec4(m_direction, 0.0f));
}

// PointLight ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PointLight::sendToShader(Shader shader)
{
    //shader.setInt("pointLight.on", m_on);
    shader.setVec3("pointLight.position", Global::view * glm::vec4(m_position, 0.0f));
    shader.setVec3("pointLight.color", m_color);
    shader.setFloat("pointLight.strength", m_strength);
    shader.setInt("pointLight.depthMap", m_depthMap);
}

void PointLight::updatePosition(Shader shader)
{
    shader.setVec3("pointLight.position", Global::view * glm::vec4(m_position, 0.0f));
}

// SpotLight ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SpotLight::sendToShader(Shader shader)
{
    //shader.setInt("spotLight.on", m_on);
    shader.setVec3("spotLight.color", m_color); //////// naam
    shader.setVec3("spotLight.position", Global::view * glm::vec4(m_position, 1.0f));
    shader.setVec3("spotLight.direction", Global::view * glm::vec4(m_direction, 0.0f));
    shader.setVec3("spotLight.color", m_color);
    shader.setFloat("spotLight.strength", m_strength);
    shader.setInt("spotLight.depthMap", m_depthMap);
    shader.setFloat("spotLight.outerCutOff", m_outerCutOff);
    shader.setFloat("spotLight.epsilon", m_innerCutOff - m_outerCutOff);
    shader.setFloat("spotLight.constant", 1.0f);
    shader.setFloat("spotLight.linear", 0.014f);
    shader.setFloat("spotLight.quadratic", 0.07f);
}

void SpotLight::updatePosition(Shader shader)
{
    shader.setVec3("spotLight.position", Global::view * glm::vec4(m_position, 1.0f));
}

void SpotLight::updateDirection(Shader shader)
{
    shader.setVec3("spotLight.direction", Global::view * glm::vec4(m_direction, 0.0f));
}

void SpotLight::updateColor(Shader shader)
{
    shader.setVec3("spotLight.color", Global::view * glm::vec4(m_color, 0.0f));
}

// FlashLight ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FlashLight::sendToShader(Shader shader)
{
    shader.setInt("flashLight.on", m_on);
    shader.setVec3("flashLight.color", m_color); //////// naam
    //shader.setVec3("flashLight.position", Global::view * glm::vec4(m_position, 0.0f));
    //shader.setVec3("flashLight.direction", Global::view * glm::vec4(m_direction, 0.0f));
    shader.setVec3("flashLight.color", m_color);
    shader.setFloat("flashLight.strength", m_strength);
    shader.setInt("flashLight.depthMap", m_depthMap);
    shader.setFloat("flashLight.outerCutOff", m_outerCutOff);
    shader.setFloat("flashLight.epsilon", m_innerCutOff - m_outerCutOff);
    shader.setFloat("flashLight.constant", 1.0f);
    shader.setFloat("flashLight.linear", 0.014f);
    shader.setFloat("flashLight.quadratic", 0.07f);
    shader.setVec3("flashLight.origin", m_offset);
    //shader.setFloat("flashLight.emissionStrength", m_emissionStrength); // TODO
}

void FlashLight::updatePosition(Shader shader)
{
    shader.setVec3("flashLight.position", Global::view * glm::vec4(m_position, 1.0f));
}

void FlashLight::updateDirection(Shader shader)
{
    shader.setVec3("flashLight.direction", Global::view * glm::vec4(m_direction, 0.0f));
}

void FlashLight::toggle(Shader shader)
{
    m_on = !m_on;
    shader.setInt("flashLight.on", m_on);
}