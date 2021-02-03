#pragma once

#include <maths/math.h>

#include <scene/GameComponent.h>
#include <util/Transform.h>
#include <scene/SceneCamera.h>
#include <scene/ScriptableObject.h>
#include <renderer/Texture2D.h>
#include <renderer/Lighting.h>

#include <box2d/b2_body.h>

namespace Engine
{

class Mesh;
class Material;
class TrueTypeFont;
class Script;
/*
struct Transform : public GameComponent
{
    Transform()
        : scale(1.f) {}

    Transform(const math::vec3& translation, const math::vec3& rotation, const math::vec3& scale)
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
*/
struct Tag : public GameComponent
{
    Tag(const std::string& str)
        : tag(str) {}
        
    std::string tag = "";
};
/*
struct CameraComponent : public GameComponent
{
    SceneCamera camera;
    bool primary;
};
*/
struct SpriteRendererComponent : public GameComponent
{
    SpriteRendererComponent()
    {
        texture = Texture2D::createWhiteTexture();
    }

    math::vec4 color = { 1.f, 1.f, 1.f, 1.f };
    Reference<Texture2D> texture;

    bool usingTexRect = false;
    math::frect textureRect;
};

struct TextRendererComponent : public GameComponent
{
    math::vec4 color;
    Reference<TrueTypeFont> font;
    std::string text;
};

class NativeScript : public GameComponent
{
public:
    ScriptableObject* instance = nullptr;

    ScriptableObject*(*instantiateScript)();
    void (*destroyScript)(NativeScript*);

    template<typename T, typename... Args>
    void bind(Args&&... args)
    {
        instantiateScript = [&args...]()
        {
            return static_cast<ScriptableObject*>(new T(std::forward<Args>(args)...));
        };

        destroyScript = [](NativeScript* component)
        {
            delete component->instance;
            component->instance = nullptr;
        };
    }
};

struct MeshComponent : public GameComponent
{
    Reference<Mesh> mesh = nullptr;
    std::string filePath = "";
    uint32_t meshID = 0;
};
/*
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
*/
struct MeshRendererComponent : public GameComponent
{
    //std::vector<Reference<Material>> materials;
    Reference<Material> material;
    bool castShadows = true;
};
/*
struct Script : public GameComponent
{
    std::string filepath = "";
    Reference<Script> script;
    bool initialized = false;
};
*/
struct RigidBody2DComponent : public GameComponent
{
    b2Body* body;
};

struct BoxCollider2DComponent : public GameComponent
{
    
};

}