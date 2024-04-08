#pragma once

#include "Global.h"
#include "Mesh.h"
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
    void Draw(Shader& shader);

private:
    std::vector<SPtr<Texture>> m_texturesLoaded{};
    std::vector<Mesh> m_meshes{};
    std::string m_directory{};
    std::string m_pathAndFileName{};
    bool m_gammaCorrection{ false };

    void loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    void loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName, std::vector<SPtr<Texture>>& meshTextures);
};