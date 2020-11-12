#pragma once

#include <string>

#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>

class Mesh
{
public:
    Mesh() {};

    Shared<VertexBuffer> vertexBuffer;
    Shared<IndexBuffer> indexBuffer;
    Shared<VertexArray> vertexArray;
};