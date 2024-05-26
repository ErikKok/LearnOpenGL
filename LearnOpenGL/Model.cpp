#pragma once

#include "Model.h"

#include "Global.h"
#include "Mesh.h"
#include "Renderer.h" // for Material
#include "Shader.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <print>
#include <vector>

Model::Model(std::string const& path, bool gamma)
    : m_gammaCorrection{ gamma }
    , m_pathAndFileName{ path }
{
    std::println("CREATE Model: {}", path);
    loadModel(path);
}
#pragma warning( suppress : 4100 )
void Model::Draw(const Material& material, const Shader* shader) const
{
    //GLint returnData{};
    //glGetIntegerv(GL_CURRENT_PROGRAM, &returnData);
    //assert(returnData == static_cast<GLint>(material.shader.getId()) && "Wrong shader active");
    //if (returnData != static_cast<GLint>(material.shader.getId()))
    
    shader->useShader();

    // Bind all unique textures to a texture unit, so they are ready to use
    // Using TU 16 to 31 (always starting from 16, so only one model can be loaded at once -> TODO)
    for (unsigned int i{ 0u }; i < m_texturesLoaded.size(); i++)
    {
        assert(i <= 15 && "Model uses > 16 textures, this is not supported!");
        if (m_texturesLoaded[i]->getBound() == -1) {
            // activate proper texture unit (i) and bind texture
            m_texturesLoaded[i]->bind(i+16);
            // save TU in texture
            m_texturesLoaded[i]->setBound(i+16);
        }
        //std::println("DRAW Texture bind #{}", i)
    }
    
    for (unsigned int i{ 0u }; i < m_meshes.size(); i++)
    {
        m_meshes[i].Draw(material);
        //std::println("DRAW Model call #{}", i);
    }

    //// You could unbind after each call, so you can call this function for a second model... quick fix
    //for (unsigned int i{ 0u }; i < m_texturesLoaded.size(); i++)
    //{
    //    assert(i <= 15 && "Model uses > 16 textures, this is not supported!");
    //    if (m_texturesLoaded[i]->getBound() != -1) {
    //        m_texturesLoaded[i]->unbindTexture();
    //    }
    //}
}

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void Model::loadModel(std::string const& path)
{
    //std::println("START Model loadModel");
    // read file via ASSIMP
    Assimp::Importer importer{};
    const aiScene* scene{ importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace) };
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) { // if is Not Zero
        std::println("ERROR::ASSIMP:: {}", importer.GetErrorString());
        return;
    }
    // retrieve the directory path of the filepath
    m_directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode* node, const aiScene* scene)
{
    //std::println("START Model processNode");
    // process each mesh located at the current node
    for (unsigned int i{ 0u }; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh{ scene->mMeshes[node->mMeshes[i]] };
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i{ 0u }; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    //std::println("START Model processMesh");
    std::vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);

    // walk through each of the mesh's vertices
    for (unsigned int i{ 0u }; i < mesh->mNumVertices; i++)
    {
        Vertex vertex{};
        glm::vec3 vector{}; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // process positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // process normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // process texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
            // process tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;
            // process bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // process indices - walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices
    std::vector<unsigned int> indices{};
    indices.reserve(mesh->mNumFaces * 3); // 3 == face.mNumIndices
    for (unsigned int i{ 0u }; i < mesh->mNumFaces; i++)
    {
        aiFace face{ mesh->mFaces[i] };
        assert(face.mNumIndices == 3);
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j{ 0u }; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<SPtr<Texture>> meshTextures{};
    meshTextures.reserve(4); // magic number

    // we assume a convention for sampler names in the shaders. Each texture should be named as '<typename>N' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER
    // 1. diffuse maps
    loadMaterialTextures(material, aiTextureType_DIFFUSE, textureType::diffuse, meshTextures);
    // 2. specular maps
    loadMaterialTextures(material, aiTextureType_SPECULAR, textureType::specular, meshTextures);
    // 3. normal maps
    //loadMaterialTextures(material, aiTextureType_HEIGHT, textureType::normal, meshTextures);
    // 4. height maps
    //loadMaterialTextures(material, aiTextureType_AMBIENT, textureType::height, meshTextures);

    return Mesh(vertices, indices, meshTextures);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
void Model::loadMaterialTextures(aiMaterial* material, aiTextureType aiTextureType, textureType textureType, std::vector<SPtr<Texture>>& meshTextures)
{
    //std::println("START Model loadMaterialTextures");
    m_texturesLoaded.reserve(4); // magic number
    bool alreadyLoaded{};
    for (unsigned int i{ 0u }; i < material->GetTextureCount(aiTextureType); i++)
    {
        aiString textureFilename{};
        material->GetTexture(aiTextureType, i, &textureFilename);

        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        alreadyLoaded = false;
        for (unsigned int j{ 0u }; j < m_texturesLoaded.size(); j++) // skipped when no textures loaded
        {
            if (std::strcmp(m_texturesLoaded[j]->getfileName().data(), textureFilename.C_Str()) == 0) { // equal
                // Create a shared_ptr from the original shared_ptr Texture and store it for each mesh to use
                meshTextures.push_back(m_texturesLoaded[j]);
                alreadyLoaded = true;
                break;
            }
        }
        if (!alreadyLoaded) {
            bool convertToLinearSpace{ false };
            if (textureType == textureType::diffuse)
                convertToLinearSpace = true;
            auto texture{ std::make_shared<Texture>(m_directory + "\\" + textureFilename.C_Str(), convertToLinearSpace) };
            texture->setType(textureType);
            texture->setfileName(textureFilename.C_Str());

            // Store each shared_ptr<Texture> in the model object
            m_texturesLoaded.push_back(texture);

            // Create a shared_ptr from the original shared_ptr<Texture> and store it for first mesh with that texture to use
            meshTextures.push_back(m_texturesLoaded.back());
        }
    }
    return;
}