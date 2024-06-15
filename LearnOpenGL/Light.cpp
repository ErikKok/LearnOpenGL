#pragma once

#include "Light.h"

// DirectionalLight //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DirectionalLight::sendToShader(const Shader& shader) const
{
    shader.useShader();
    //shader.setInt("dirLight.on", m_on);
    shader.setVec3("dirLightDirection", glm::normalize(Global::camera.getViewMatrix() * glm::vec4(m_direction, 0.0f)));
    shader.setVec3("dirLight.color", m_color);
    shader.setFloat("dirLight.strength", m_strength);
    shader.setFloat("dirLight.ambient", m_ambient);
    shader.setInt("dirLight.depthMap", m_depthMap);
}

void DirectionalLight::updateDirectionInViewSpace(const Shader& shader) const
{
    shader.useShader(); 
    shader.setVec3("dirLightDirection", glm::normalize(Global::camera.getViewMatrix() * glm::vec4(m_direction, 0.0f)));
}

// PointLight ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PointLight::sendToShader(const Shader& shader) const
{
    shader.useShader();
    //shader.setInt("pointLight.on", m_on);
    shader.setVec3("pointLightPosition", Global::camera.getViewMatrix() * glm::vec4(m_position, 0.0f));
    shader.setVec3("pointLight.color", m_color);
    shader.setFloat("pointLight.strength", m_strength);
    shader.setInt("pointLight.depthMap", m_depthMap);
}

void PointLight::updatePositionInViewSpace(const Shader& shader) const
{
    shader.useShader(); 
    shader.setVec3("pointLightPosition", Global::camera.getViewMatrix() * glm::vec4(m_position, 0.0f));
}

// SpotLight ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SpotLight::sendToShader(const Shader& shader) const
{
    shader.useShader();
    //shader.setInt("spotLight.on", m_on);
    shader.setVec3("spotLight.color", m_color); //////// naam
    shader.setVec3("spotLightPosition", Global::camera.getViewMatrix() * glm::vec4(m_position, 1.0f));
    shader.setVec3("spotLight.direction", Global::camera.getViewMatrix() * glm::vec4(m_direction, 0.0f));
    shader.setVec3("spotLight.color", m_color);
    shader.setFloat("spotLight.strength", m_strength);
    shader.setInt("spotLight.depthMap", m_depthMap);
    shader.setFloat("spotLight.outerCutOff", m_outerCutOff);
    shader.setFloat("spotLight.epsilon", m_innerCutOff - m_outerCutOff);
    shader.setFloat("spotLight.constant", 1.0f);
    shader.setFloat("spotLight.linear", 0.014f);
    shader.setFloat("spotLight.quadratic", 0.07f);
}

void SpotLight::updatePositionInViewSpace(const Shader& shader) const
{
    shader.useShader(); 
    shader.setVec3("spotLightPosition", Global::camera.getViewMatrix() * glm::vec4(m_position, 1.0f));
}

void SpotLight::updateDirectionInViewSpace(const Shader& shader) const
{
    shader.useShader(); 
    shader.setVec3("spotLight.direction", Global::camera.getViewMatrix() * glm::vec4(m_direction, 0.0f));
}

void SpotLight::updateColor(const Shader& shader) const
{
    shader.useShader(); 
    shader.setVec3("spotLight.color", glm::vec4(m_color, 0.0f));
}

// FlashLight ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FlashLight::sendToShader(const Shader& shader) const
{
    shader.useShader(); 
    shader.setInt("flashLight.on", m_on);
    shader.setVec3("flashLight.color", m_color); //////// naam TODO?
    //shader.setVec3("flashLight.position", Global::camera.getViewMatrix() * glm::vec4(m_position, 0.0f));         // TODO
    //shader.setVec3("flashLight.direction", Global::camera.getViewMatrix() * glm::vec4(m_direction, 0.0f));       // TODO direction staat hard in de shader (cameraDirection)
    shader.setVec3("flashLight.color", m_color);
    shader.setFloat("flashLight.strength", m_strength);
    shader.setInt("flashLight.depthMap", m_depthMap);
    shader.setFloat("flashLight.outerCutOff", m_outerCutOff);
    shader.setFloat("flashLight.epsilon", m_innerCutOff - m_outerCutOff);
    shader.setFloat("flashLight.constant", 1.0f);
    shader.setFloat("flashLight.linear", 0.014f);
    shader.setFloat("flashLight.quadratic", 0.07f);
    shader.setVec3("flashLight.origin", m_origin);
    shader.setFloat("flashLight.emissionStrength", m_emissionStrength);
}
 
//void FlashLight::updatePositionInViewSpace(const Shader& shader) const         // TODO
//{
//    shader.useShader(); 
//    shader.setVec3("flashLight.position", Global::camera.getViewMatrix() * glm::vec4(m_position, 1.0f));
//}
//
//void FlashLight::updateDirectionInViewSpace(const Shader& shader) const         // TODO
//{
//    shader.useShader();
//    //glm::vec3 dir = Global::camera.getViewMatrix() * glm::vec4(m_direction, 0.0f);
//    //shader.setVec3("flashLight.direction", { dir.x, dir.y, -dir.z });
//    shader.setVec3("flashLight.direction", Global::camera.getViewMatrix() * glm::vec4(m_direction, 0.0f));
//}

void FlashLight::toggle(const Shader& shader, const Shader& shader2)
{
    m_on = !m_on;
    shader.useShader(); 
    shader.setInt("flashLight.on", m_on);
    shader2.useShader();
    shader2.setInt("flashLight.on", m_on);
}