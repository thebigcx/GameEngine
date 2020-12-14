#pragma once

#include <maths/math.h>

#include <util/Transform.h>
#include <util/OrthographicCameraController.h>
#include <scene/SceneCamera.h>
#include <scene/ScriptableEntity.h>
#include <renderer/Texture2D.h>
#include <renderer/text/TrueTypeFont.h>

struct TransformComponent
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

struct TagComponent
{
    TagComponent(const std::string& str)
        : tag(str) {}
        
    std::string tag;
};

struct CameraComponent
{
    SceneCamera camera;
    bool primary;
};

struct SpriteRendererComponent
{
    math::vec4 color = { 1.f, 1.f, 1.f, 1.f };
    Shared<Texture2D> texture;

    bool usingTexRect = false;
    math::frect textureRect;
};

struct TextRendererComponent
{
    math::vec4 color;
    Shared<TrueTypeFont> font;
    std::string text;
};

struct NativeScriptComponent
{
    ScriptableEntity* instance = nullptr;

    ScriptableEntity*(*instantiateScript)();
    void (*destroyScript)(NativeScriptComponent*);

    template<typename T>
    void bind()
    {
        instantiateScript = []()
        {
            return static_cast<ScriptableEntity*>(new T());
        };

        destroyScript = [](NativeScriptComponent* component)
        {
            delete component->instance;
            component->instance = nullptr;
        };
    }
};