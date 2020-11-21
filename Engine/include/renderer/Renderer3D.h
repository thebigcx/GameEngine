#pragma once

#include <math/matrix/mat4.h>
#include <renderer/Mesh.h>
#include <renderer/Texture2D.h>
#include <renderer/Material.h>
#include <renderer/Model.h>

struct Renderer3DData
{
    math::mat4 projectionMatrix;
    Shared<Shader> modelShader;
};

class Renderer3D
{
public:
    static void init();

    static void render(const Mesh& mesh, const math::mat4& transform, const Shared<Material>& material);
    static void submit(const Shared<Model>& model, const math::mat4& transform);

    static Renderer3DData data;
};