#pragma once

#include <string>

#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>
#include <renderer/Material.h>
#include <core/Core.h>

namespace Engine
{

class Mesh
{
public:
    Mesh() {};

    Shared<VertexBuffer> vertexBuffer;
    Shared<IndexBuffer> indexBuffer;
    Shared<VertexArray> vertexArray;
    Shared<Material> material;

    void setVertices(const std::vector<math::vec3>& positions, const std::vector<math::vec3>& uvs, const std::vector<uint32_t>& indices);

    static Shared<Mesh> load(const std::string& path, unsigned int id);
};

}