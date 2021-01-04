#include <renderer/InstancedRenderer.h>
#include <renderer/RenderCommand.h>

namespace Engine
{

void InstancedRenderer::init()
{
    glCreateBuffers(1, &s_data.instanceBuffer); // TODO: platform independence
    //glBindBuffer(GL_ARRAY_BUFFER, s_data.instanceBuffer);
}

void InstancedRenderer::add(const math::mat4& transform)
{
    s_data.transforms.push_back(transform);
}

void InstancedRenderer::setMesh(const Shared<Mesh>& mesh)
{
    s_data.mesh = mesh;

    s_data.mesh->vertexArray->bind();
    
    glBindBuffer(GL_ARRAY_BUFFER, s_data.instanceBuffer);

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

void InstancedRenderer::render()
{
    glBindBuffer(GL_ARRAY_BUFFER, s_data.instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, s_data.transforms.size() * sizeof(math::mat4), &s_data.transforms[0], GL_STATIC_DRAW);

    if (!s_data.mesh)
    {
        return;
    }

    s_data.mesh->material->bind();

    s_data.mesh->vertexArray->bind();

    RenderCommand::renderInstanced(s_data.mesh->vertexArray, s_data.transforms.size());
}

};