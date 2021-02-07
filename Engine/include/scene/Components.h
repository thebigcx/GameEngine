#pragma once

#include <maths/math.h>

#include <scene/GameComponent.h>
#include <renderer/Texture2D.h>

namespace Engine
{

class Mesh;
class Material;
class TrueTypeFont;
class Script;

struct Tag : public GameComponent
{
    Tag(const std::string& str)
        : tag(str) {}
        
    std::string tag = "";
};

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
/*
struct TextRendererComponent : public GameComponent
{
    math::vec4 color;
    Reference<TrueTypeFont> font;
    std::string text;
};
*/
struct MeshComponent : public GameComponent
{
    Reference<Mesh> mesh = nullptr;
};

struct MeshRendererComponent : public GameComponent
{
    Reference<Material> material;
    bool castShadows = true;
};
/*
struct RigidBody2DComponent : public GameComponent
{
    b2Body* body;
};

struct BoxCollider2DComponent : public GameComponent
{
    
};
*/
}