#pragma once

#include <core/Core.h>
#include <renderer/Mesh.h>

class MeshFactory
{
public:
    static Shared<Mesh> textMesh();
};