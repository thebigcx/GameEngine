#pragma once

#include <maths/matrix/mat4.h>
#include <renderer/Mesh.h>
#include <renderer/Texture2D.h>
#include <renderer/Material.h>
#include <renderer/Model.h>
#include <renderer/Lights.h>

#include <util/PerspectiveCamera.h>

struct Renderer3DData
{
    math::mat4 projectionMatrix;
    Shared<Shader> modelShader;
    PerspectiveCamera* camera;
    bool sceneStarted = false;
};

class Renderer3D
{
public:
    static void init();

    static void beginScene(PerspectiveCamera& camera);

    static void render(const Mesh& mesh, const math::mat4& transform);
    static void submit(const Shared<Model>& model, const math::mat4& transform);

    static void setLights(const LightSetup& setup);

    static void endScene();

    static Renderer3DData data;
};