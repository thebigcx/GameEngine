#pragma once

#include <math/matrix/mat4.h>
#include <renderer/Mesh.h>
#include <renderer/Texture2D.h>
#include <renderer/Material.h>

struct Renderer3DData
{
    math::mat4 projectionMatrix;
};

class Renderer3D
{
public:
    static void init();

    static void render(const Shared<Mesh>& mesh, const math::mat4& transform, const Shared<Material>& material);

    static Renderer3DData data;
};