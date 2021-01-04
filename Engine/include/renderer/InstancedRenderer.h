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
    void setInstance(const std::vector<Shared<Mesh>>& model);
    void setInstance(const Shared<Mesh>& mesh);

    void updateInstances();
    void resetInstances();

    const std::vector<Shared<Mesh>>& getInstance() const { return m_meshes; }
    const std::vector<RenderingInstance>& getInstances() const { return m_instances; }

    static Shared<InstancedRenderer> create();

private:
    std::vector<RenderingInstance> m_instances;
    std::vector<Shared<Mesh>> m_meshes;

    Shared<VertexBuffer> m_instanceBuffer;

    bool m_instanceChanged = false;

    void setInstance_(const Shared<Mesh>& mesh);
};

}