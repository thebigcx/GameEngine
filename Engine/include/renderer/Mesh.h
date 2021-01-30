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

    Reference<VertexBuffer> vertexBuffer;
    Reference<IndexBuffer> indexBuffer;
    Reference<VertexArray> vertexArray;
    Reference<Material> material;

    void setVertices(const std::vector<math::vec3>& positions, const std::vector<math::vec3>& uvs, const std::vector<uint32_t>& indices);

    static Reference<Mesh> load(const std::string& path, unsigned int id);
};

}