#pragma once

#include <maths/matrix/mat4x4.h>
#include <renderer/Mesh.h>
#include <renderer/Texture2D.h>
#include <renderer/Material.h>
#include <renderer/Model.h>
#include <renderer/Lighting.h>
#include <renderer/Framebuffer.h>
#include <renderer/Skybox.h>
#include <renderer/InstancedRenderer.h>
#include <renderer/EnvironmentMap.h>

#include <util/PerspectiveCamera.h>
#include <scene/EditorCamera.h>

namespace Engine
{

struct RenderObject
{
    Shared<Mesh> mesh;
    math::mat4 transform;
};

struct Renderer3DData
{
    bool sceneStarted = false;

    Shared<UniformBuffer> matrixData;

    Shared<EnvironmentMap> environment;
    Shared<Shader> environmentShader;
    Shared<Mesh> skyboxMesh;

    Shared<Texture2D> shadowMap;
    Shared<Framebuffer> shadowMapFramebuffer;
    math::mat4 lightProjection;
    math::mat4 lightView;
    math::mat4 lightMatrix;

    std::vector<const BaseLight*> lights;

    math::vec3 cameraPos;
    
    std::unordered_map<Shared<Material>, std::vector<RenderObject>> renderObjects;

    bool usingSkybox = true;
};

class Renderer3D
{
public:
    static void beginScene(PerspectiveCamera& camera);
    static void beginScene(EditorCamera& camera);
    static void beginScene(Camera& camera, const math::mat4& transform);

    static void submit(const Shared<Mesh>& mesh, const math::mat4& transform); // TODO: meshes shouldn't hold materials (research further)
    static void submit(const Shared<Model>& model, const math::mat4& transform);
    static void submit(const Shared<Mesh>& mesh, const math::mat4& transform, const Shared<Material>& material);
    static void submitOutline(const Shared<Mesh>& mesh, const math::mat4& transform, const math::vec3& outlineColor);

    static void submit(const Shared<InstancedRenderer>& instance);

    static void setEnvironment(const Shared<EnvironmentMap>& environment);

    static void startBatch();
    static void flushBatch();
    static void nextBatch();

    static inline void addLight(const BaseLight* light) { s_data.lights.push_back(light); }
    static inline void clearLights() { std::vector<const BaseLight*>().swap(s_data.lights); }

    static void removeLight(const BaseLight* light);

    static void endScene();

    static void renderShadows();

    static void useSkybox(bool use) { s_data.usingSkybox = use; }

private:
    static void setLightingUniforms(const Shared<Shader>& shader);

    static void init();
    static void shutdown();

    static inline Renderer3DData s_data;

    friend class Renderer;
};

}