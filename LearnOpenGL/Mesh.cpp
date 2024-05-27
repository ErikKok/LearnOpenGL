#pragma once

#include "mesh.h"

#include "Data.h"
#include "Global.h" 
#include "Renderer.h" // for Material
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <memory> // for std::unique_ptr and std::make_unique
#include <print>
#include <vector>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<SPtr<Texture>> textures)
    :m_vertices{vertices}
    ,m_indices{indices}
    ,m_textures{textures}
    ,m_vao{ std::make_unique<VertexArray>() }
    ,m_vbo{ std::make_unique<VertexBuffer>(m_vertices.size() * sizeof(Vertex), &m_vertices[0]) }
    ,m_ebo{ std::make_unique<ElementBuffer>(m_indices.size() * sizeof(unsigned int), &m_indices[0]) }
    ,m_layout{ std::make_unique<VertexAttributeLayout>() }
{
    //std::println("CREATE Mesh");
    setupMesh();
}

void Mesh::Draw(const Material& material) const
{
    // Set sampler2D uniforms to the correct texture unit for each texture in this Mesh

    unsigned int diffuseCount{ 1u };
    unsigned int specularCount{ 1u };
    //unsigned int normalCount = 1u; // TODO
    //unsigned int heightCount = 1u; // TODO

    for (unsigned int i{ 0u }; i < m_textures.size(); i++)
    {
        assert(m_textures[i]->getBound() >= 0 && "Texture is not bound to a texture unit");
        
        // retrieve texture number (the N in <typename>N)
        std::string count{};
        textureType textureType{ m_textures[i]->getType() }; // textureType
        if (textureType == textureType::diffuse)
            count = std::to_string(diffuseCount++);
        else if (textureType == textureType::specular)
            count = std::to_string(specularCount++); // transfer unsigned int to string
        else if (textureType == textureType::normal) // TODO
            break;
            //count = std::to_string(normalCount++); // transfer unsigned int to string
        else if (textureType == textureType::height) // TODO
            break;
            //count = std::to_string(heightCount++); // transfer unsigned int to string

        std::string result{ "material." + m_textures[i]->getTypeAsString() + count };
        if (!FrameBuffer::s_depthMapPassActive)
            material.shader.setInt(result, m_textures[i]->getBound());   
    };

    m_vao->bindVertexArray();

    // set remaining material properties
    if (!FrameBuffer::s_depthMapPassActive) {
        material.shader.setInt("material.emission", material.emission);
        material.shader.setFloat("material.emissionStrength", material.emissionStrength);
        material.shader.setFloat("material.shininess", material.shininess);
        material.shader.setInt("material.flashLightEmissionMap", material.flashLightEmissionMap);
        material.shader.setInt("material.flashLightEmissionTexture", material.flashLightEmissionTexture);
    }

    glVertexArrayVertexBuffer(m_vao->getId(), 0, m_vbo->getId(), 0, m_layout->getStride());
    glVertexArrayElementBuffer(m_vao->getId(), m_ebo->getId());

    // dit werkte nadat ebo weer gebind wordt.

    // m_ebo->getCount()
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(m_vertices.size()), GL_UNSIGNED_INT, 0, 1);
}

void Mesh::setupMesh() const
{
    //std::println("SETUP Mesh");
    // No need to bind anything, constructor already took care of that
    m_layout->pushVertexAttributeLayout<float>(3);      // 0 - positions
    m_layout->pushVertexAttributeLayout<float>(2);      // 1 - tex coords
    m_layout->pushVertexAttributeLayout<float>(3);      // 2 - normals
    m_layout->pushVertexAttributeLayout<float>(3);      // 3 - Tangent
    m_layout->pushVertexAttributeLayout<float>(3);      // 4 - Bitangent
    //m_layout->pushVertexAttributeLayout<int>(4);      // 5 - BoneIDs
    //m_layout->pushVertexAttributeLayout<float>(4);    // 6 - Weights
    //m_layout->setVertexStride(88);
    m_vao->addVertexAttributeLayout(*m_vbo, *m_layout);
}