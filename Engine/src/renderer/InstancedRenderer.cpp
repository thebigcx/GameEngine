#include <renderer/InstancedRenderer.h>
#include <renderer/RenderCommand.h>

namespace Engine
{

InstancedRenderer::InstancedRenderer()
{
    m_instanceBuffer = VertexBuffer::create();
}

void InstancedRenderer::add(const RenderingInstance& instance)
{
    m_instances.push_back(instance);
}

void InstancedRenderer::setInstance_(const Shared<Mesh>& mesh)
{
    m_meshes.push_back(mesh);

    mesh->vertexArray->bind();
    
    m_instanceBuffer->bind();

    glEnableVertexAttribArray(4);// TODO: platform independence
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(math::vec4), (void*)0);
    glEnableVertexAttribArray(5); 
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(math::vec4), (void*)(1 * sizeof(math::vec4)));
    glEnableVertexAttribArray(6); 
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(math::vec4), (void*)(2 * sizeof(math::vec4)));
    glEnableVertexAttribArray(7); 
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(math::vec4), (void*)(3 * sizeof(math::vec4)));

    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);
}

void InstancedRenderer::setInstance(const Shared<Mesh>& mesh)
{
    setInstance_(mesh);
}

void InstancedRenderer::setInstance(const std::vector<Shared<Mesh>>& meshes)
{
    for (auto& mesh : meshes)
    {
        setInstance_(mesh);
    }
}

void InstancedRenderer::resetInstances()
{
    m_instances.clear();
}

void InstancedRenderer::updateInstances()
{
    m_instanceBuffer->setData(math::buffer(m_instances[0].transform), m_instances.size() * sizeof(math::mat4));
}

Shared<InstancedRenderer> InstancedRenderer::create()
{
    return createShared<InstancedRenderer>();
}

};