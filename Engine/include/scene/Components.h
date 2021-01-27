#pragma once

#include <maths/math.h>

#include <util/Transform.h>
#include <util/OrthographicCameraController.h>
#include <scene/SceneCamera.h>
#include <scene/ScriptableObject.h>
#include <renderer/Texture2D.h>
#include <renderer/text/TrueTypeFont.h>
#include <renderer/Mesh.h>
#include <renderer/Lighting.h>
#include <renderer/Assets.h>
#include <script/CSharpScript.h>

#include <box2d/b2_body.h>

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
    SpriteRendererComponent()
    {
        texture = Texture2D::createWhiteTexture();
    }

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
    ScriptableObject* instance = nullptr;

    ScriptableObject*(*instantiateScript)();
    void (*destroyScript)(NativeScriptComponent*);

    template<typename T, typename... Args>
    void bind(Args&&... args)
    {
        instantiateScript = [&args...]()
        {
            return static_cast<ScriptableObject*>(new T(std::forward<Args>(args)...));
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
    Shared<Mesh> mesh = nullptr;
    std::string filePath = "";
    uint32_t meshID = 0;
};

struct DirectionalLightComponent : public GameComponent
{
    DirectionalLight light;
};

struct SkyLightComponent : public GameComponent
{
    SkyLight light;
};

struct PointLightComponent : public GameComponent
{
    PointLight light;
};

struct MeshRendererComponent : public GameComponent
{
    //std::vector<Shared<Material>> materials;
    Shared<Material> material;
    bool castShadows = true;
};

struct CSharpScriptComponent : public GameComponent
{
    std::string filepath = "";
    Shared<CSharpScript> script;
    bool initialized = false;
};

struct RigidBody2DComponent : public GameComponent
{
    b2Body* body;
};

struct BoxCollider2DComponent : public GameComponent
{
    
};

}