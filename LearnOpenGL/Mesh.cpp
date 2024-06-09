#pragma once

#include "mesh.h"

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
    :m_verticesFloat{ vertices }
    ,m_indices{ indices }
    ,m_vao{ std::make_unique<VertexArray>() }
    ,m_vbo{ std::make_unique<VertexBuffer>(m_verticesFloat.size() * sizeof(float), &m_verticesFloat[0]) } // can not use sizeof() with vector
    ,m_ebo{ std::make_unique<ElementBuffer>(m_indices.size() * sizeof(unsigned int), &m_indices[0]) } // can not use sizeof() with vector
    ,m_layout{ std::make_unique<VertexAttributeLayout>() }
{
    setupMesh323();
    //std::println("CREATE Mesh");
}

void Mesh::setupMesh323() const // TODO rename setupVBO323
{
    // Specify layout of vbo and bind it to vao
    m_layout->pushVertexAttributeLayout<float>(3);      // 0 - positions
    m_layout->pushVertexAttributeLayout<float>(2);      // 1 - tex coords
    m_layout->pushVertexAttributeLayout<float>(3);      // 2 - normals
    m_vao->finalizeVertexAttributeLayout(m_vbo.get(), m_layout.get());

    // Bind ebo to vao
    glVertexArrayElementBuffer(m_vao->getId(), m_ebo->getId());

    //std::println("SETUP Mesh");
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<SPtr<Texture>>& textures)
    :m_vertices{ vertices }
    ,m_indices{ indices }
    ,m_textures{ textures }
    ,m_vao{ std::make_unique<VertexArray>() }
    ,m_vbo{ std::make_unique<VertexBuffer>(m_vertices.size() * sizeof(Vertex), &m_vertices[0]) }
    ,m_ebo{ std::make_unique<ElementBuffer>(m_indices.size() * sizeof(GLuint), &m_indices[0])}
    ,m_layout{ std::make_unique<VertexAttributeLayout>() }
{
    setupMesh32333();
    //std::println("CREATE Mesh");
}

void Mesh::setupMesh32333() const
{
    // Specify layout of vbo and bind it to vao
    m_layout->pushVertexAttributeLayout<float>(3);      // 0 - positions
    m_layout->pushVertexAttributeLayout<float>(2);      // 1 - tex coords
    m_layout->pushVertexAttributeLayout<float>(3);      // 2 - normals
    m_layout->pushVertexAttributeLayout<float>(3);      // 3 - Tangent
    m_layout->pushVertexAttributeLayout<float>(3);      // 4 - Bitangent
    //m_layout->pushVertexAttributeLayout<int>(4);      // 5 - BoneIDs
    //m_layout->pushVertexAttributeLayout<float>(4);    // 6 - Weights
    //m_layout->setVertexStride(88);
    m_vao->finalizeVertexAttributeLayout(m_vbo.get(), m_layout.get());

    // Bind ebo to vao
    glVertexArrayElementBuffer(m_vao->getId(), m_ebo->getId());

    //std::println("SETUP Mesh");

    //assert(glm::dot(glm::cross(m_vertices[0].normal, m_vertices[0].tangent), m_vertices[0].bitangent) >= 0.0f && "Texture coordinates are oriented in the wrong way!");
}