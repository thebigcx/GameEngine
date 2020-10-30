#pragma once

#include <string>

#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>

class Mesh
{
public:
    Mesh() {};

    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    VertexArray vertexArray;
};