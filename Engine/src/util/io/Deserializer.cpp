#include <util/io/Deserializer.h>
#include <renderer/Material.h>
#include <renderer/Assets.h>
#include <renderer/Mesh.h>
#include <scene/Scene.h>
#include <scene/Components.h>
#include <util/Transform.h>
#include <scene/SceneCamera.h>
#include <renderer/Lighting.h>

namespace Engine
{

Reference<Material> Deserializer::loadMaterial(const std::string& path)
{
    YAML::Node meta = YAML::LoadFile(path + ".meta");
    std::string uuid = meta["uuid"].as<std::string>();

    YAML::Node root = YAML::LoadFile(path);
    auto matNode = root["Material"];

    std::string name = matNode["Name"].as<std::string>();
    std::string shaderUUID = matNode["Shader"]["uuid"].as<std::string>();

    auto material = Material::create(/* Get shader by UUID */);

    material->albedoMap = Assets::get<Texture2D>(matNode["Textures"]["Albedo"]["uuid"].as<std::string>());
    material->normalMap = Assets::get<Texture2D>(matNode["Textures"]["Normal"]["uuid"].as<std::string>());
    material->metallicMap = Assets::get<Texture2D>(matNode["Textures"]["Metallic"]["uuid"].as<std::string>());
    material->roughnessMap = Assets::get<Texture2D>(matNode["Textures"]["Roughness"]["uuid"].as<std::string>());
    material->ambientOcclusionMap = Assets::get<Texture2D>(matNode["Textures"]["Ambient Occlusion"]["uuid"].as<std::string>());
    material->emissionMap = Assets::get<Texture2D>(matNode["Textures"]["Emission"]["uuid"].as<std::string>());

    return material;
}

Reference<Texture2D> Deserializer::loadTexture(const std::string& path)
{
    YAML::Node meta = YAML::LoadFile(path + ".meta");
    std::string uuid = meta["uuid"].as<std::string>();
    bool clamp = meta["Texture"]["Clamp"].as<bool>();
    bool linear = meta["Texture"]["Linear"].as<bool>();
    bool isSRGB = meta["Texture"]["IsSRGB"].as<bool>();

    return Texture2D::create(path, clamp, linear, isSRGB);
}

Reference<Shader> Deserializer::loadShader(const std::string& path)
{
    YAML::Node meta = YAML::LoadFile(path + ".meta");

    return Shader::createFromFile(path);
}

Reference<Mesh> Deserializer::loadMesh(const std::string& path)
{
    YAML::Node meta = YAML::LoadFile(path + ".meta");

    return Mesh::load(path, 0);
}

Reference<Scene> Deserializer::loadScene(const std::string& path)
{
    YAML::Node node = YAML::LoadFile(path);

    auto scene = Scene::create();
    scene->setPath(path);

    for (YAML::const_iterator it = node.begin(); it != node.end(); it++)
    {
        auto object = node[it->first];

        loadGameObject(scene->getRootGameObject(), object);
    }

    return scene;
}

void Deserializer::loadGameObject(GameObject& parent, YAML::Node& node)
{
    auto object = parent.createChild();
    object->createComponent<Tag>(node["Name"].as<std::string>());

    if (node["Transform"])
    {
        auto transform = object->createComponent<Transform>();

        transform->setTranslation(node["Transform"]["Translation"][0].as<float>(), 
                                  node["Transform"]["Translation"][1].as<float>(), 
                                  node["Transform"]["Translation"][2].as<float>());

        transform->setRotation(node["Transform"]["Rotation"][0].as<float>(), 
                               node["Transform"]["Rotation"][1].as<float>(), 
                               node["Transform"]["Rotation"][2].as<float>());

        transform->setScale(node["Transform"]["Scale"][0].as<float>(), 
                            node["Transform"]["Scale"][1].as<float>(), 
                            node["Transform"]["Scale"][2].as<float>());
    }

    if (node["Camera"])
    {
        auto camera = object->createComponent<SceneCamera>();

        camera->setProjectionType(static_cast<Camera::ProjectionType>(node["Camera"]["Projection Type"].as<uint32_t>()));

        camera->setOrthoSize(node["Camera"]["Ortho Size"].as<float>());
        camera->setOrthoNear(node["Camera"]["Ortho Near"].as<float>());
        camera->setOrthoFar(node["Camera"]["Ortho Far"].as<float>());

        camera->setPerspectiveFov(node["Camera"]["Perspective Fov"].as<float>());
        camera->setPerspectiveNear(node["Camera"]["Perspective Near"].as<float>());
        camera->setPerspectiveFar(node["Camera"]["Perspective Far"].as<float>());

        camera->primary = node["Camera"]["Primary"].as<bool>();
    }

    if (node["Sprite Renderer"])
    {
        auto sr = object->createComponent<SpriteRendererComponent>();

        sr->color.r = node["Sprite Renderer"]["Color"][0].as<float>();
        sr->color.g = node["Sprite Renderer"]["Color"][1].as<float>();
        sr->color.b = node["Sprite Renderer"]["Color"][2].as<float>();
        sr->color.a = node["Sprite Renderer"]["Color"][3].as<float>();

        if (node["Sprite Renderer"]["Texture"].as<std::string>() != "")
        {
            sr->texture = Assets::get<Texture2D>(node["Sprite Renderer"]["Texture"].as<std::string>());
        }

        sr->usingTexRect = node["Sprite Renderer"]["Using Texture Rect"].as<bool>();

        sr->textureRect.x = node["Sprite Renderer"]["Texture Rect"][0].as<float>();
        sr->textureRect.y = node["Sprite Renderer"]["Texture Rect"][1].as<float>();
        sr->textureRect.w = node["Sprite Renderer"]["Texture Rect"][2].as<float>();
        sr->textureRect.h = node["Sprite Renderer"]["Texture Rect"][3].as<float>();
    }

    if (node["Mesh"])
    {
        auto mesh = object->createComponent<MeshComponent>();

        mesh->mesh = Mesh::load(node["Mesh"]["Mesh"].as<std::string>(), node["Mesh"]["Mesh ID"].as<uint32_t>()); // TODO: put meshes in Asset cache
    }

    if (node["Mesh Renderer"])
    {
        auto meshRenderer = object->createComponent<MeshRendererComponent>();

        std::string material = node["Mesh Renderer"]["Material"].as<std::string>();

        if (material != "")
        {
            meshRenderer->material = Assets::get<Material>(material);
        }
    }

    if (node["Directional Light"])
    {
        auto light = object->createComponent<DirectionalLight>();

        light->radiance.r = node["Directional Light"]["Radiance"][0].as<float>();
        light->radiance.g = node["Directional Light"]["Radiance"][1].as<float>();
        light->radiance.b = node["Directional Light"]["Radiance"][2].as<float>();

        light->intensity = node["Directional Light"]["Intensity"].as<float>();
    }

    if (node["Sky Light"])
    {
        auto light = object->createComponent<SkyLight>();

        light->intensity = node["Sky Light"]["Intensity"].as<float>();

        light->radiance.r = node["Sky Light"]["Radiance"][0].as<float>();
        light->radiance.g = node["Sky Light"]["Radiance"][1].as<float>();
        light->radiance.b = node["Sky Light"]["Radiance"][2].as<float>();
    }

    if (node["Point Light"])
    {
        auto light = object->createComponent<PointLight>();

        light->radiance.r = node["Point Light"]["Radiance"][0].as<float>();
        light->radiance.g = node["Point Light"]["Radiance"][1].as<float>();
        light->radiance.b = node["Point Light"]["Radiance"][2].as<float>();

        light->intensity = node["Point Light"]["Intensity"].as<float>();
    }

    if (node["Children"])
    {
        for (YAML::const_iterator it = node["Children"].begin(); it != node["Children"].end(); it++)
        {
            auto child = node["Children"][it->first];
            loadGameObject(*object, child);
        }
    }
}

}