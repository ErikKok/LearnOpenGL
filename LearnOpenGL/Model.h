#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

class Model

{
public:
    friend class Renderer;

    Model(std::string const& path);
    Model(const Model& other) = delete;					// Copy constructor deleted: mesh is not copyable
    Model& operator=(const Model& other) = delete;		// Copy assignment
    Model(Model&& other) noexcept = default; 			// Move constructor	
    Model& operator=(Model&& other) noexcept = default;	// Move assignment

    // int getUniqueTexturesCount() return m_texturesLoaded.size();

private:
    std::vector<Mesh> m_meshes{};
    std::vector<SPtr<Texture>> m_texturesLoaded{}; // wordt alleen gebruikt tijdens inladen v/e model toch? kan als static buiten de class worden gehouden dus?
    std::string m_directory{};
    std::string m_pathAndFileName{};

    void loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    void loadMaterialTextures(aiMaterial* material, aiTextureType aiTextureType, textureType textureType, std::vector<SPtr<Texture>>& meshTextures);
};