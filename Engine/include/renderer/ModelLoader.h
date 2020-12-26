#pragma once

#include <renderer/Model.h>

namespace Engine
{

class ModelLoader
{
public:
    static Shared<Model> load(const std::string& path);
    static Shared<Mesh> loadMesh(const std::string& path, unsigned int id);

private:
    static void processNode_(aiNode* node, const aiScene* scene, const Shared<Model>& model);
    static Shared<Mesh> processMesh_(aiMesh* mesh, const aiScene* scene, const Shared<Model>& model);
    static Shared<Texture2D> loadMaterialTexture_(aiMaterial* mat, aiTextureType type, const std::string& directory);

    static const aiScene* setupAssimp_(const std::string& modelPath);

    static inline std::vector<Shared<Texture2D>> m_texturesLoaded;
    static inline std::unordered_map<int, Shared<Material>> m_materialsLoaded;
    static inline std::unordered_map<std::string, Shared<Model>> m_modelsLoaded;

    static inline Assimp::Importer m_importer;
};

}