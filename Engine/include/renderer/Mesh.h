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

    static Shared<Mesh> load(const std::string& path, unsigned int id);
};

}