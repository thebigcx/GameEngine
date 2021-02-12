#include <util/io/Serializer.h>
#include <renderer/Mesh.h>
#include <scene/Scene.h>
#include <scene/Components.h>
#include <script/Script.h>
#include <renderer/Lighting.h>
#include <util/Transform.h>
#include <scene/SceneCamera.h>

namespace Engine
{

void Serializer::saveMaterial(const Reference<Material>& material, const std::string& path)
{
    YAML::Node meta;
    meta["uuid"] = material->uuid;

    YAML::Node root;
    auto matNode = root["Material"];
    
    matNode["Name"] = material->name;
    matNode["Shader"]["Name"] = material->shader->name;

    {
        auto textureNode = matNode["Textures"];
        textureNode["Albedo"]["Name"] = material->albedoMap->name;
        textureNode["Normal"]["Name"] = material->normalMap->name;
        textureNode["Metallic"]["Name"] = material->metallicMap->name;
        textureNode["Roughness"]["Name"] = material->roughnessMap->name;
        textureNode["Ambient Occlusion"]["Name"] = material->ambientOcclusionMap->name;
        textureNode["Height"]["Name"] = material->depthMap->name;
        textureNode["Emission"]["Name"] = material->emissionMap->name;
    }

    std::ofstream metaFile(path + material->name + ".material.meta");
    metaFile << meta;

    std::ofstream file(path + material->name + ".material");
    file << root;
}

void Serializer::saveTexture(const Reference<Texture2D>& texture)
{
    YAML::Node meta;
    meta["Name"] = texture->name;
    meta["Texture"]["Clamp"] = texture->isClamped();
    meta["Texture"]["Linear"] = texture->isLinear();
    meta["Texture"]["IsSRGB"] = texture->isSRGB();

    std::ofstream metaFile(texture->getPath() + ".meta");
    metaFile << meta;
}

void Serializer::saveShader(const Reference<Shader>& shader)
{
    YAML::Node meta;
    
    std::ofstream metaFile(shader->getPath() + ".meta");
    metaFile << meta;
}

void Serializer::saveMesh(const Reference<Mesh>& mesh)
{
    YAML::Node node;

    //std::ofstream metaFile(mesh->file)
}

void Serializer::saveScript(const Reference<Script>& script)
{
    YAML::Node meta;

    std::ofstream metaFile(script->getPath());
    metaFile << meta;
}

void Serializer::saveGameObject(YAML::Node& node, GameObject& object)
{
    node["Name"] = object.getComponent<Tag>()->tag;

    if (object.hasComponent<Transform>())
    {
        auto tc = object.getComponent<Transform>();
        auto transform = node["Transform"];

        transform["Translation"].push_back<float>(tc->getTranslation().x);
        transform["Translation"].push_back<float>(tc->getTranslation().y);
        transform["Translation"].push_back<float>(tc->getTranslation().z);
        transform["Translation"].SetStyle(YAML::EmitterStyle::Flow);
        
        transform["Rotation"].push_back<float>(tc->getRotation().x);
        transform["Rotation"].push_back<float>(tc->getRotation().y);
        transform["Rotation"].push_back<float>(tc->getRotation().z);
        transform["Rotation"].SetStyle(YAML::EmitterStyle::Flow);

        transform["Scale"].push_back<float>(tc->getScale().x);
        transform["Scale"].push_back<float>(tc->getScale().y);
        transform["Scale"].push_back<float>(tc->getScale().z);
        transform["Scale"].SetStyle(YAML::EmitterStyle::Flow);
    }

    if (object.hasComponent<SceneCamera>())
    {
        auto comp = object.getComponent<SceneCamera>();
        auto camera = node["Camera"];

        camera["Projection Type"] = static_cast<uint32_t>(comp->getProjectionType());

        camera["Ortho Size"] = comp->getOrthoSize();
        camera["Ortho Near"] = comp->getOrthoNear();
        camera["Ortho Far"] = comp->getOrthoFar();

        camera["Perspective Fov"] = comp->getPerspectiveFov();
        camera["Perspective Near"] = comp->getPerspectiveNear();
        camera["Perspective Far"] = comp->getPerspectiveFar();

        camera["Aspect"] = comp->getAspect();
        camera["Primary"] = comp->primary;
    }

    if (object.hasComponent<SpriteRendererComponent>())
    {
        auto comp = object.getComponent<SpriteRendererComponent>();
        auto spriteRenderer = node["Sprite Renderer"];

        spriteRenderer["Color"].push_back<float>(comp->color.r);
        spriteRenderer["Color"].push_back<float>(comp->color.g);
        spriteRenderer["Color"].push_back<float>(comp->color.b);
        spriteRenderer["Color"].push_back<float>(comp->color.a);
        spriteRenderer["Color"].SetStyle(YAML::EmitterStyle::Flow);

        if (comp->texture)
            spriteRenderer["Texture"] = comp->texture->name;
        else
            spriteRenderer["Texture"] = "none";

        spriteRenderer["Using Texture Rect"] = comp->usingTexRect;

        spriteRenderer["Texture Rect"].push_back<float>(comp->textureRect.x);
        spriteRenderer["Texture Rect"].push_back<float>(comp->textureRect.y);
        spriteRenderer["Texture Rect"].push_back<float>(comp->textureRect.w);
        spriteRenderer["Texture Rect"].push_back<float>(comp->textureRect.h);
        spriteRenderer["Texture Rect"].SetStyle(YAML::EmitterStyle::Flow);
    }

    if (object.hasComponent<MeshComponent>())
    {
        auto comp = object.getComponent<MeshComponent>();
        auto mesh = node["Mesh"];

        mesh["Mesh"] = comp->mesh->path;
        mesh["Mesh ID"] = comp->mesh->id;
    }

    if (object.hasComponent<MeshRendererComponent>())
    {
        auto comp = object.getComponent<MeshRendererComponent>();
        auto meshRenderer = node["Mesh Renderer"];

        meshRenderer["Material"] = comp->material ? comp->material->name : "";
    }

    if (object.hasComponent<DirectionalLight>())
    {
        auto comp = object.getComponent<DirectionalLight>();
        auto light = node["Directional Light"];

        light["Radiance"][0] = comp->radiance.r;
        light["Radiance"][1] = comp->radiance.g;
        light["Radiance"][2] = comp->radiance.b;

        light["Intensity"] = comp->intensity;
    }

    if (object.hasComponent<PointLight>())
    {
        auto comp = object.getComponent<PointLight>();
        auto light = node["Point Light"];

        light["Radiance"][0] = comp->radiance.r;
        light["Radiance"][1] = comp->radiance.g;
        light["Radiance"][2] = comp->radiance.b;

        light["Intensity"] = comp->intensity;
    }

    if (object.hasComponent<SkyLight>())
    {
        auto comp = object.getComponent<SkyLight>();
        auto light = node["Sky Light"];

        light["Intensity"] = comp->intensity;
        
        light["Radiance"][0] = comp->radiance.r;
        light["Radiance"][1] = comp->radiance.g;
        light["Radiance"][2] = comp->radiance.b;
    }

    for (auto child : object.getChildren())
    {
        auto childNode = node["Children"][child->getComponent<Tag>()->tag];
        saveGameObject(childNode, *child);
    }
}

void Serializer::saveScene(const Reference<Scene>& scene, const std::string& path)
{
    YAML::Node root;

    for (auto gameObject : scene->getRootGameObject().getChildren())
    {
        auto objectNode = root[gameObject->getComponent<Tag>()->tag];
        saveGameObject(objectNode, *gameObject);
    }

    std::ofstream file(path);
    file << root;
}

}