#pragma once

#include "mesh.h"
#include <memory> // for std::unique_ptr and std::make_unique


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures)
    :m_vertices{vertices}
    ,m_indices{indices}
    ,m_textures{textures}
    ,m_vao{ new VertexArray{} }
    ,m_vbo{ new VertexBuffer(m_vertices.size() * sizeof(Vertex), &m_vertices[0])}
    ,m_ebo{ new ElementBuffer(m_indices.size() * sizeof(unsigned int), &m_indices[0])}
    ,m_layout{ new VertexAttributeLayout{} }
{
    //std::println("CREATE Mesh");
    setupMesh();
}

void Mesh::setupMesh()
{
    //std::println("SETUP Mesh");
    // No need to bind anything, constructor did that
    m_layout->pushVertexAttributeLayout<float>(3);      // 0 - positions
    m_layout->pushVertexAttributeLayout<float>(2);      // 1 - tex coords
    m_layout->pushVertexAttributeLayout<float>(3);      // 2 - normals
    m_layout->pushVertexAttributeLayout<float>(3);      // 3 - Tangent
    m_layout->pushVertexAttributeLayout<float>(3);      // 4 - Bitangent
    //m_layout->pushVertexAttributeLayout<int>(4);      // 5 - BoneIDs
    //m_layout->pushVertexAttributeLayout<float>(4);    // 6 - Weights
    m_vao->addVertexAttributeLayout(*m_vbo, *m_layout);
}

void Mesh::Draw(Shader& shader)
{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    //unsigned int normalNr = 1; // TODO
    //unsigned int heightNr = 1; // TODO
    for (unsigned int i = 0; i < m_textures.size(); i++)
    {
        // retrieve texture number (the N in <typename>N)
        std::string number;
        std::string name = m_textures[i]->m_type;
        if (name == "diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string 
        else if (name == "normal") // TODO
            break;
            //number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "height") // TODO
            break;
            //number = std::to_string(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        std::string result{ "material." + name + number };
        shader.setInt(result, i); // m_id
        // activate proper texture unit (i) and bind the texture
        m_textures[i]->bindTexture(i);
    }
    // draw mesh
    m_vao->bindVertexArray();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_vertices.size()), GL_UNSIGNED_INT, 0);
}