#pragma once

#include <string>
#include <vector>

#include <core/Core.h>
#include <renderer/Mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine
{

struct ModelVertex
{
    math::vec3 position;
    math::vec3 normal;
    math::vec2 uv;
    math::vec3 tangent;
};

class Model
{
public:
    static Shared<Model> loadModel(const std::string& file);
    static Shared<Mesh> loadMeshAtID(const std::string& file, unsigned int id);

    std::vector<Shared<Mesh>> meshes;

    std::string path;

private:
    std::string m_directory;

    void processNode(aiNode* node, const aiScene* scene, const Shared<Model>& model);
    Shared<Mesh> processMesh(aiMesh* mesh, const aiScene* scene, const Shared<Model>& model);
    Shared<Texture2D> loadMaterialTexture(aiMaterial* mat, aiTextureType type);

    std::vector<Shared<Texture2D>> m_texturesLoaded;
    std::unordered_map<int, Shared<Material>> m_materialsLoaded;
};

}