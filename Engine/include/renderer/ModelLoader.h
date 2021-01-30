#pragma once

#include <renderer/Model.h>

namespace Engine
{

class ModelLoader
{
public:
    static Reference<Model> load(const std::string& path);
    static Reference<Mesh> loadMesh(const std::string& path, unsigned int id);

private:
    static void processNode_(aiNode* node, const aiScene* scene, const Reference<Model>& model);
    static Reference<Mesh> processMesh_(aiMesh* mesh, const aiScene* scene, const Reference<Model>& model);
    static Reference<Texture2D> loadMaterialTexture_(aiMaterial* mat, aiTextureType type, const std::string& directory);

    static const aiScene* setupAssimp_(const std::string& modelPath);

    static inline std::vector<Reference<Texture2D>> m_texturesLoaded;
    static inline std::unordered_map<int, Reference<Material>> m_materialsLoaded;
    static inline std::unordered_map<std::string, Reference<Model>> m_modelsLoaded;

    static inline Assimp::Importer m_importer;
};

}