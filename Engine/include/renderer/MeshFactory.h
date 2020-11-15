#pragma once

#include <core/Core.h>
#include <renderer/Mesh.h>

class MeshFactory
{
public:
    static Shared<Mesh> textMesh();
    static Shared<Mesh> quadMesh(float x, float y, float x1, float y1);

    static Shared<Mesh> cubeMesh(float size);
};