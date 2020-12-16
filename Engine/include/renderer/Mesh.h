#pragma once

#include <string>

#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>
#include <renderer/Material.h>

class Mesh
{
public:
    Mesh() {};

    Shared<VertexBuffer> vertexBuffer;
    Shared<IndexBuffer> indexBuffer;
    Shared<VertexArray> vertexArray;
    //Shared<Material> material;
    std::vector<Shared<Material>> materials;
};