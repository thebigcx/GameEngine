#pragma once

#include <core/Core.h>
#include <renderer/Mesh.h>

namespace Engine
{

class MeshFactory
{
public:
    static Shared<Mesh> textMesh(uint32_t charCount);
    static Shared<Mesh> quadMesh(float x, float y, float x1, float y1);
    static Shared<Mesh> cubeMesh(float size);
    static Shared<Mesh> sphereMesh(float radius, int sectors, int stacks);

    static math::vec3 calculateTangent(
        const math::vec3& pos1,
        const math::vec3& pos2, 
        const math::vec3& pos3,
        const math::vec2& uv1,
        const math::vec2& uv2,
        const math::vec2& uv3);

    static math::vec3 calculateNormal(
        const math::vec3& pos1,
        const math::vec3& pos2,
        const math::vec3& pos3);

    static Shared<Mesh> skyboxMesh();
};

}