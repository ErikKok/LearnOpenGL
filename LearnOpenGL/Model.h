#pragma once

#include "Global.h"
#include "Mesh.h"
//#include "Renderer.h" // for Material
#include "Shader.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

class Model
{
public:
    Model(std::string const& path, bool gamma = false);
    Model(const Model& other) = delete;					// Copy constructor deleted: mesh is not copyable
    Model& operator=(const Model& other) = delete;		// Copy assignment
    Model(Model&& other) noexcept = default; 			// Move constructor	
    Model& operator=(Model&& other) noexcept = default;	// Move assignment
    
    //void draw(const RenderObject& RO, const Renderer& renderer) const;

    // int getUniqueTexturesCount() return m_texturesLoaded.size();
    std::vector<Mesh> m_meshes{}; // TODO
    std::vector<SPtr<Texture>> m_texturesLoaded{}; // TODO
private:


    std::string m_directory{};
    std::string m_pathAndFileName{};
    bool m_gammaCorrection{ false };

    void loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    void loadMaterialTextures(aiMaterial* material, aiTextureType aiTextureType, textureType textureType, std::vector<SPtr<Texture>>& meshTextures);
};