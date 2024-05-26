#pragma once

#include "Buffers.h"
#include "Global.h"
#include "Renderer.h" // for Material
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

#include <memory> // for std::unique_ptr and std::make_unique
#include <print>
#include <vector>

//#define MAX_BONE_INFLUENCE 4
struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoords;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    //int m_BoneIDs[MAX_BONE_INFLUENCE]; // bone indexes which will influence this vertex
    //float m_Weights[MAX_BONE_INFLUENCE]; // weights from each bone
};

class Mesh {
public:                                            
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<SPtr<Texture>> textures); // Constructor
    Mesh(const Mesh& other) = delete;		        	// Copy constructor
    Mesh& operator=(const Mesh& other) = delete;	    // Copy assignment
    Mesh(Mesh&& other) noexcept = default;				// Move constructor
    Mesh& operator=(Mesh&& other) noexcept = default;	// Move assignment
    ~Mesh() = default;									// Destructor

    void Draw(const Material& material) const;

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<SPtr<Texture>> m_textures;
    std::unique_ptr<VertexArray> m_vao{ nullptr };
    std::unique_ptr<VertexBuffer> m_vbo{ nullptr };
    std::unique_ptr<ElementBuffer> m_ebo{ nullptr };
    std::unique_ptr<VertexAttributeLayout> m_layout{ nullptr };

    void setupMesh() const;
};