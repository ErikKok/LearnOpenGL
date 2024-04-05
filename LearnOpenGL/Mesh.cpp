#pragma once

#include "mesh.h"
#include <memory> // for std::unique_ptr and std::make_unique


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures)
    :m_vertices{vertices}
    ,m_indices{indices}
    ,m_textures{textures}
    ,m_vao{ new VertexArray{} }
    ,m_vbo{ new VertexBuffer(m_vertices.size() * sizeof(Vertex), &m_vertices[0])}
    ,m_ebo{ new ElementBuffer(m_indices.size() * sizeof(unsigned int), &m_indices[0])}
    ,m_layout{ new VertexAttributeLayout{} }
{
    std::println("CREATE Mesh");
    setupMesh();
}

void Mesh::setupMesh()
{
    std::println("SETUP Mesh");
    Global::glCheckError();

    GLint returnData;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &returnData);
    std::println("Currently bound VertexArray: {}", returnData);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &returnData);
    std::println("Currently bound ElementBuffer: {}", returnData);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &returnData);
    std::println("Currently bound VertexBuffer: {}", returnData);

    //m_vao->bindVertexArray();
    //m_vbo->bindVertexBuffer();

    m_layout->pushVertexAttributeLayout<float>(3); // 0 - positions   // positions en normals andersom! NIET MEER
    m_layout->pushVertexAttributeLayout<float>(2); // 1 - tex coords
    m_layout->pushVertexAttributeLayout<float>(3); // 2 - normals
    m_layout->pushVertexAttributeLayout<float>(3); // 3 - Tangent
    m_layout->pushVertexAttributeLayout<float>(3); // 4 - Bitangent
    //m_layout->pushVertexAttributeLayout<int>(4); // 5 - BoneIDs
    //m_layout->pushVertexAttributeLayout<float>(4); // 6 - Weights
    m_vao->addVertexAttributeLayout(*m_vbo, *m_layout);

    //m_ebo->unbindElementBuffer();
}

void Mesh::Draw(Shader& shader) // bindTexture() + glDrawElements
{
    // bind appropriate textures
    unsigned int diffuseNr = 1; // TODO
    unsigned int specularNr = 1;
    //unsigned int normalNr = 1;
    //unsigned int heightNr = 1;
    for (unsigned int i = 0; i < m_textures.size(); i++) // i == texture unit
    {
        //glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = m_textures[i]->m_type;
        if (name == "diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string // TODO
        else if (name == "normal")
            break;
            //number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "height")
            break;
            //number = std::to_string(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        std::string result{ "material." + name + number };
        //shader.setInt(result, static_cast<GLint>(m_textures[i].m_Id)); // m_Id
        shader.setInt(result, i); // m_Id
        // and finally bind the texture

        //m_textures[i].bindTexture(m_textures[i].m_Id);
        m_textures[i]->bindTexture(i);
        //glBindTexture(GL_TEXTURE_2D, textures[i].m_id);
    }
    // always good practice to set everything back to defaults once configured.
    //glActiveTexture(GL_TEXTURE0); // TODO weg?

    // draw mesh
    m_vao->bindVertexArray();
    //std::println("MESH DRAW OFZO");
    //glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_vertices.size()), GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
}