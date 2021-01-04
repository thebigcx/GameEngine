#pragma once

#include <vector>

#include <renderer/Mesh.h>
#include <core/Core.h>

namespace Engine
{

struct InstancedRendererData
{
    std::vector<math::mat4> transforms;
    Shared<Mesh> mesh = nullptr;

    uint32_t instanceBuffer;// TODO: platform independence
};

class InstancedRenderer
{
public:
    static void init();

    static void setMesh(const Shared<Mesh>& mesh); // TODO: support models
    static void add(const math::mat4& transform);
    static void render();

private:
    static inline InstancedRendererData s_data;
};

}