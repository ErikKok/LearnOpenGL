#pragma once

//#include "BufferSubData.h"
//#include "Camera.h"
#include "Data.h"
#include "ElementBuffer.h"
#include "Global.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
//#include "VertexAttribute.h"
//#include "UniformBuffer.h"
//
//// External header warning level: 0
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#define STB_IMAGE_IMPLEMENTATION
//#include <STB/stb_image.h>

//#include <assimp/config.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <memory> // for std::unique_ptr and std::make_unique

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
    Mesh() {};
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<SPtr<Texture>> textures);
    void Draw(Shader& shader);

// TODO private
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<SPtr<Texture>> m_textures; // TODO textures kunnen door meerdere meshes worden gebruikt (waarschijnlijk zelfs), dan algemene storage maken en daarnaar pointen

    // over unique pointers: However, if the class object is not destroyed properly (e.g. it is dynamically allocated and not deallocated properly), then the std::unique_ptr member will not be destroyed either, and the object being managed by the std::unique_ptr will not be deallocated.
    std::unique_ptr<VertexArray> m_vao; 
    std::unique_ptr<VertexBuffer> m_vbo; // initialize hier, want anders wordt de copy constructor (en dus deconstructor) aangeroepen?!
    std::unique_ptr<ElementBuffer> m_ebo; // idem
    std::unique_ptr<VertexAttributeLayout> m_layout;
private:
    void setupMesh();
};