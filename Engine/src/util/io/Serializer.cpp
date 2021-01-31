#include <util/io/Serializer.h>
#include <renderer/Mesh.h>
#include <scene/Scene.h>
#include <scene/Components.h>
#include <script/Script.h>

#include <yaml-cpp/yaml.h>

namespace Engine
{

void Serializer::saveMaterial(const Reference<Material>& material, const std::string& path)
{
    YAML::Node meta;
    meta["uuid"] = material->uuid;

    YAML::Node root;
    auto matNode = root["Material"];
    
    matNode["Name"] = material->name;
    matNode["Shader"]["uuid"] = material->shader->uuid;

    {
        auto textureNode = matNode["Textures"];
        textureNode["Albedo"]["uuid"] = material->albedoMap->uuid;
        textureNode["Normal"]["uuid"] = material->normalMap->uuid;
        textureNode["Metallic"]["uuid"] = material->metallicMap->uuid;
        textureNode["Roughness"]["uuid"] = material->roughnessMap->uuid;
        textureNode["Ambient Occlusion"]["uuid"] = material->ambientOcclusionMap->uuid;
        textureNode["Height"]["uuid"] = material->depthMap->uuid;
        textureNode["Emission"]["uuid"] = material->emissionMap->uuid;
    }

    std::ofstream metaFile(path + material->name + ".material.meta");
    metaFile << meta;

    std::ofstream file(path + material->name + ".material");
    file << root;
}

void Serializer::saveTexture(const Reference<Texture2D>& texture)
{
    YAML::Node meta;
    meta["uuid"] = texture->uuid;
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

void serializeGameObject(YAML::Node& node, GameObject& object)
{
    node["Name"] = object.getComponent<TagComponent>().tag;

    if (object.hasComponent<TransformComponent>())
    {
        auto tc = object.getComponent<TransformComponent>();
        auto transform = node["Transform"];

        transform["Translation"].push_back<float>(tc.translation.x);
        transform["Translation"].push_back<float>(tc.translation.y);
        transform["Translation"].push_back<float>(tc.translation.z);
        transform["Translation"].SetStyle(YAML::EmitterStyle::Flow);
        
        transform["Rotation"].push_back<float>(tc.rotation.x);
        transform["Rotation"].push_back<float>(tc.rotation.y);
        transform["Rotation"].push_back<float>(tc.rotation.z);
        transform["Rotation"].SetStyle(YAML::EmitterStyle::Flow);

        transform["Scale"].push_back<float>(tc.scale.x);
        transform["Scale"].push_back<float>(tc.scale.y);
        transform["Scale"].push_back<float>(tc.scale.z);
        transform["Scale"].SetStyle(YAML::EmitterStyle::Flow);
    }

    if (object.hasComponent<CameraComponent>())
    {
        auto comp = object.getComponent<CameraComponent>();
        auto camera = node["Camera"];

        camera["Projection Type"] = static_cast<uint32_t>(comp.camera.getProjectionType());

        camera["Ortho Size"] = comp.camera.getOrthoSize();
        camera["Ortho Near"] = comp.camera.getOrthoNear();
        camera["Ortho Far"] = comp.camera.getOrthoFar();

        camera["Perspective Fov"] = comp.camera.getPerspectiveFov();
        camera["Perspective Near"] = comp.camera.getPerspectiveNear();
        camera["Perspective Far"] = comp.camera.getPerspectiveFar();

        camera["Aspect"] = comp.camera.getAspect();
        camera["Primary"] = comp.primary;
    }

    if (object.hasComponent<SpriteRendererComponent>())
    {
        auto comp = object.getComponent<SpriteRendererComponent>();
        auto spriteRenderer = node["Sprite Renderer"];

        spriteRenderer["Color"].push_back<float>(comp.color.r);
        spriteRenderer["Color"].push_back<float>(comp.color.g);
        spriteRenderer["Color"].push_back<float>(comp.color.b);
        spriteRenderer["Color"].push_back<float>(comp.color.a);
        spriteRenderer["Color"].SetStyle(YAML::EmitterStyle::Flow);

        if (comp.texture)
            spriteRenderer["Texture"] = comp.texture->uuid;
        else
            spriteRenderer["Texture"] = "none";

        spriteRenderer["Using Texture Rect"] = comp.usingTexRect;

        spriteRenderer["Texture Rect"].push_back<float>(comp.textureRect.x);
        spriteRenderer["Texture Rect"].push_back<float>(comp.textureRect.y);
        spriteRenderer["Texture Rect"].push_back<float>(comp.textureRect.w);
        spriteRenderer["Texture Rect"].push_back<float>(comp.textureRect.h);
        spriteRenderer["Texture Rect"].SetStyle(YAML::EmitterStyle::Flow);
    }

    if (object.hasComponent<MeshComponent>())
    {
        auto comp = object.getComponent<MeshComponent>();
        auto mesh = node["Mesh"];

        mesh["Mesh"] = comp.filePath;
        mesh["Mesh ID"] = comp.meshID;
    }

    if (object.hasComponent<MeshRendererComponent>())
    {
        auto comp = object.getComponent<MeshRendererComponent>();
        auto meshRenderer = node["Mesh Renderer"];

        auto& uuid = comp.material->uuid; // FIXME: null materials
        meshRenderer["Material"] = uuid;
    }

    if (object.hasComponent<DirectionalLightComponent>())
    {
        auto comp = object.getComponent<DirectionalLightComponent>();
        auto light = node["Directional Light"];

        light["Radiance"][0] = comp.light.radiance.r;
        light["Radiance"][1] = comp.light.radiance.g;
        light["Radiance"][2] = comp.light.radiance.b;

        light["Intensity"] = comp.light.intensity;
    }

    if (object.hasComponent<PointLightComponent>())
    {
        auto comp = object.getComponent<PointLightComponent>();
        auto light = node["Point Light"];

        light["Radiance"][0] = comp.light.radiance.r;
        light["Radiance"][1] = comp.light.radiance.g;
        light["Radiance"][2] = comp.light.radiance.b;

        light["Intensity"] = comp.light.intensity;
    }

    if (object.hasComponent<SkyLightComponent>())
    {
        auto comp = object.getComponent<SkyLightComponent>();
        auto light = node["Sky Light"];

        light["Intensity"] = comp.light.intensity;
        
        light["Radiance"][0] = comp.light.radiance.r;
        light["Radiance"][1] = comp.light.radiance.g;
        light["Radiance"][2] = comp.light.radiance.b;
    }

    for (auto& child : object.getChildren())
    {
        auto childNode = node["Children"][child.getComponent<TagComponent>().tag];
        serializeGameObject(childNode, child);
    }
}

void Serializer::saveScene(const Reference<Scene>& scene, const std::string& path)
{
    YAML::Node root;

    for (auto& gameObject : scene->getRootGameObject().getChildren())
    {
        auto objectNode = root[gameObject.getComponent<TagComponent>().tag];
        serializeGameObject(objectNode, gameObject);
    }

    std::ofstream file(path);
    file << root;
}

}