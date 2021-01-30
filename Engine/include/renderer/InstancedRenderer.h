#pragma once

#include <vector>

#include <renderer/Mesh.h>
#include <core/Core.h>

namespace Engine
{

struct RenderingInstance
{
    math::mat4 transform;
};

class InstancedRenderer
{
public:
    InstancedRenderer();

    void add(const RenderingInstance& instance);
    void setInstance(const std::vector<Reference<Mesh>>& model);
    void setInstance(const Reference<Mesh>& mesh);

    void updateInstances();
    void resetInstances();

    const std::vector<Reference<Mesh>>& getInstance() const { return m_meshes; }
    const std::vector<RenderingInstance>& getInstances() const { return m_instances; }

    static Reference<InstancedRenderer> create();

private:
    std::vector<RenderingInstance> m_instances;
    std::vector<Reference<Mesh>> m_meshes;

    Reference<VertexBuffer> m_instanceBuffer;

    bool m_instanceChanged = false;

    void setInstance_(const Reference<Mesh>& mesh);
};

}