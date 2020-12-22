#pragma once

#include <string>
#include <vector>

#include <core/Core.h>
#include <renderer/Mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

    std::vector<Shared<Mesh>> meshes;

    std::string path;

private:
    std::string m_directory;

    void processNode(aiNode* node, const aiScene* scene);
    Shared<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Shared<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type);

    std::vector<Shared<Texture2D>> m_texturesLoaded;
};