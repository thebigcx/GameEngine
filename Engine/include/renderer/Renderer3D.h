#pragma once

#include <math/matrix/mat4.h>
#include <renderer/Mesh.h>
#include <renderer/Texture2D.h>

struct Renderer3DData
{
    math::mat4 projectionMatrix;
};

class Renderer3D
{
public:
    static void init();

    static void render(const Mesh& mesh, const math::mat4& transform, const Shared<Texture2D>& texture, const Shared<Shader>& shader);

    static Renderer3DData data;
};