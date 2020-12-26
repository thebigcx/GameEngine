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
    static Shared<Model> load(const std::string& file);

    std::vector<Shared<Mesh>> meshes;

    std::string path;
    std::string directory;
};

}