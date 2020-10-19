#pragma once

#include <renderer/Texture.h>
#include <renderer/Shader.h>

class RenderStates
{
public:
    RenderStates()
    {
        
    }

    static RenderStates createStates(Texture& texture_, Shader& shader_, const glm::mat4& transform_)
    {
        RenderStates states;
        states.texture = &texture_;
        states.shader = &shader_;
        states.transform = transform_;

        return states;
    }

    void bind() const
    {
        texture->bind();
        shader->bind();
        shader->setUniform("transform", transform);
    }

    Texture* texture;
    Shader* shader;
    glm::mat4 transform;
};