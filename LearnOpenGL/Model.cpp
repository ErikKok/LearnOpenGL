#pragma once

#include "Model.h"

#include "Global.h"
#include "Mesh.h"
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

Model::Model(std::string const& path)
    : m_pathAndFileName{ path }
{
    std::println("CREATE Model: {}", path);
    loadModel(path);
}

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void Model::loadModel(std::string const& path)
{
    //std::println("START Model loadModel");
    // read file via ASSIMP
    Assimp::Importer importer{};
    const aiScene* scene{ importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace) };
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
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
        //aiMesh* mesh{ scene->mMeshes[node->mMeshes[i]] };
        m_meshes.emplace_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
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

        // https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/ Handedness
        if (glm::dot(glm::cross(vertex.normal, vertex.tangent), vertex.bitangent) < 0.0f) {
            vertex.tangent = vertex.tangent * -1.0f;
            //std::println("Texcoords oriented in the wrong way: corrected!");
        }

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
            indices.emplace_back(face.mIndices[j]);
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
    loadMaterialTextures(material, aiTextureType_HEIGHT, textureType::normal, meshTextures); // The wavefront object format (.obj) exports normal maps slightly different from Assimp's conventions as aiTextureType_NORMAL doesn't load normal maps, while aiTextureType_HEIGHT does
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
        for (auto j{ 0 }; j < std::ssize(m_texturesLoaded); j++) // skipped when no textures loaded
        {
            if (std::strcmp(m_texturesLoaded[j]->getfileName().data(), textureFilename.C_Str()) == 0) { // equal
                // Create a shared_ptr from the original shared_ptr Texture and store it for each mesh to use
                meshTextures.emplace_back(m_texturesLoaded[j]);
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


    // TEST 15-6-2024
    //assert(m_texturesLoaded.size() >= 1 && "Model contains no textures!");
    
    //// Set default textures
    //if(m_texturesLoaded.size() == 0) {
    //    // diffuse
    //    auto texture2{ std::make_shared<Texture>("Textures\\dev_orange.png", true)};
    //    texture2->setType(textureType::diffuse);
    //    texture2->setfileName("dev_orange.png");
    //        
    //    m_texturesLoaded.push_back(texture2);
    //    meshTextures.push_back(m_texturesLoaded.back());
    //}

    return;
}