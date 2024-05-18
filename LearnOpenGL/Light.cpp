#pragma once

#include "Light.h"

void DirectionalLight::sendToShader(Shader shader)
{
    shader.setVec3("dirLight.diffuse", m_color);
    shader.setFloat("dirLight.strength", m_strength);
    shader.setFloat("dirLight.ambient", m_ambient);
    shader.setInt("dirLight.depthMap", m_depthMap);
}

//SpotLight::SpotLight(glm::vec3 position)
//    : Light{ position }
//{};