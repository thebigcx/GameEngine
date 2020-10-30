#pragma once

#include <renderer/Texture2D.h>
#include <renderer/shader/Shader.h>
#include <renderer/BlendMode.h>

class RenderStates
{
public:
    RenderStates()
    {
        
    }

    static RenderStates createStates(Texture2D& texture_, Shader& shader_, const Matrix4f& transform_, BlendMode blendMode_)
    {
        RenderStates states;
        states.texture = &texture_;
        states.shader = &shader_;
        states.transform = transform_;
        states.blendMode = blendMode_;

        return states;
    }

    static RenderStates createStates(Texture2D& texture_, Shader& shader_)
    {
        RenderStates states;
        states.texture = &texture_;
        states.shader = &shader_;
        states.transform = Matrix4f(1.f);

        return states;
    }

    void bind() const
    {
        texture->bind();
        shader->bind();
        shader->setUniform("transform", transform);
        blendMode.bind();
    }

    Texture2D* texture;
    Shader* shader;
    Matrix4f transform;
    BlendMode blendMode;
};