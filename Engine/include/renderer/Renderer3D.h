#pragma once

#include <maths/matrix/mat4x4.h>
#include <renderer/Mesh.h>
#include <renderer/Texture2D.h>
#include <renderer/Material.h>
#include <renderer/Model.h>
#include <renderer/Lights.h>
#include <renderer/Framebuffer.h>
#include <renderer/Skybox.h>

#include <util/PerspectiveCamera.h>
#include <scene/EditorCamera.h>

namespace Engine
{

struct Renderer3DData
{
    Shared<Shader> modelShader;

    bool sceneStarted = false;
    Shared<UniformBuffer> lightingData;

    Shared<UniformBuffer> matrixData;

    Shared<Skybox> environment;
    Shared<Mesh> skyboxMesh;
    Shared<Shader> skyboxShader;
};

class Renderer3D
{
public:
    static void init();
    static void shutdown();

    static void beginScene(PerspectiveCamera& camera);
    static void beginScene(EditorCamera& camera);

    static void submit(const Shared<Mesh>& mesh, const math::mat4& transform);
    static void submit(const Shared<Model>& model, const math::mat4& transform);
    static void submit(const Shared<Mesh>& mesh, const math::mat4& transform, const Shared<Material>& material);

    static void setLights(const LightSetup& setup);
    static void setEnvironment(const Shared<Skybox>& environment);

    static void endScene();

    static Renderer3DData data;
};

}