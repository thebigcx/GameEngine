#pragma once

#include <maths/math.h>

#include <util/Transform.h>
#include <util/OrthographicCameraController.h>
#include <scene/SceneCamera.h>
#include <scene/ScriptableGameObject.h>
#include <renderer/Texture2D.h>
#include <renderer/text/TrueTypeFont.h>
#include <renderer/Mesh.h>
#include <renderer/Lights.h>

namespace Engine
{

struct TransformComponent : public GameComponent
{
    TransformComponent()
        : scale(1.f) {}

    TransformComponent(const math::vec3& translation, const math::vec3& rotation, const math::vec3& scale)
        : translation(translation), rotation(rotation), scale(scale) {}

    math::vec3 translation;
    math::vec3 rotation;
    math::vec3 scale;

    math::mat4 getTransform()
    {
        Transform transform = { translation, rotation, scale };
        return transform.matrix();
    }
};

struct TagComponent : public GameComponent
{
    TagComponent(const std::string& str)
        : tag(str) {}
        
    std::string tag = "";
};

struct CameraComponent : public GameComponent
{
    SceneCamera camera;
    bool primary;
};

struct SpriteRendererComponent : public GameComponent
{
    math::vec4 color = { 1.f, 1.f, 1.f, 1.f };
    Shared<Texture2D> texture;

    bool usingTexRect = false;
    math::frect textureRect;
};

struct TextRendererComponent : public GameComponent
{
    math::vec4 color;
    Shared<TrueTypeFont> font;
    std::string text;
};

struct NativeScriptComponent : public GameComponent
{
    ScriptableGameObject* instance = nullptr;

    ScriptableGameObject*(*instantiateScript)();
    void (*destroyScript)(NativeScriptComponent*);

    template<typename T>
    void bind()
    {
        instantiateScript = []()
        {
            return static_cast<ScriptableGameObject*>(new T());
        };

        destroyScript = [](NativeScriptComponent* component)
        {
            delete component->instance;
            component->instance = nullptr;
        };
    }
};

struct MeshComponent : public GameComponent
{
    Shared<Mesh> mesh;
    std::string filePath;
    uint32_t meshID = 0; // TODO: mesh id
};

struct BoxCollider2DComponent : public GameComponent
{
    math::frect box;
};

struct DirectionalLightComponent : public GameComponent
{
    math::vec3 radiance = math::vec3(1.f);
    float intensity = 0.5f;
};

struct SkyLightComponent : public GameComponent
{
    float intensity = 0.5f;
};

struct PointLightComponent : public GameComponent
{
    math::vec3 radiance = math::vec3(1.f);
    float intensity = 0.5f;
    float attenuation = 0.5f;
};

struct MeshRendererComponent : public GameComponent
{
    std::vector<Shared<Material>> materials;
};

struct LuaScriptComponent : public GameComponent
{
    std::string filePath;
    std::string source;
};

}