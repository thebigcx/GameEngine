#pragma once

#include <string>

#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>
#include <renderer/Material.h>

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
};

}