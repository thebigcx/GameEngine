/*#include <scene/SceneSerializer.h>

#include <scene/Components.h>
#include <renderer/Model.h>
#include <renderer/Assets.h>
#include <util/Timer.h>

#include <fstream>

namespace Engine
{

// TODO: this whole file needs to be revised ASAP.

void SceneSerializer::saveScene(const Reference<Scene>& scene, const std::string& path)
{
    YAML::Node root;
    YAML::Node sceneNode = root["Scene"];
    
    auto objects = root["Scene"]["GameObjects"];

    // Materials
    for (auto& material : Assets::getCache<Material>())
    {
        auto& name = material.first;
        auto& mat = material.second;

        if (mat->albedoMap)
            sceneNode["Materials"][name]["Albedo"] = mat->albedoMap->getPath();
        else
            sceneNode["Materials"][name]["Albedo"] = "";


        if (mat->normalMap)
            sceneNode["Materials"][name]["Normal"] = mat->normalMap->getPath();
        else
            sceneNode["Materials"][name]["Normal"] = "";


        if (mat->metallicMap)
            sceneNode["Materials"][name]["Metallic"] = mat->metallicMap->getPath();
        else
            sceneNode["Materials"][name]["Metallic"] = "";

        
        if (mat->roughnessMap)
            sceneNode["Materials"][name]["Roughness"] = mat->roughnessMap->getPath();
        else
            sceneNode["Materials"][name]["Roughness"] = "";


        if (mat->ambientOcclusionMap)
            sceneNode["Materials"][name]["Ambient Occlusion"] = mat->ambientOcclusionMap->getPath();
        else
            sceneNode["Materials"][name]["Ambient Occlusion"] = "";


        if (mat->depthMap)
            sceneNode["Materials"][name]["Depth"] = mat->depthMap->getPath();
        else
            sceneNode["Materials"][name]["Depth"] = "";


        sceneNode["Materials"][name]["Shader"] = Assets::find<Shader>(mat->shader);
    }

    for (auto& texture : Assets::getCache<Texture2D>())
    {
        if (texture.first == "white_texture")
            sceneNode["Texture2Ds"][texture.first] = "white_texture";
        else
            sceneNode["Texture2Ds"][texture.first] = texture.second->getPath();
    }

    for (auto& shader : Assets::getCache<Shader>())
    {
        sceneNode["Shaders"][shader.first] = shader.second->getPath();
    }

    auto& children = scene->getRootGameObject().getChildren();

    for (auto& object : children)
    {
        auto objectNode = objects[object.getComponent<Tag>().tag];

        saveGameObject(object, objectNode);
    }

    std::ofstream fout(path);
    fout << root;
}

Reference<Scene> SceneSerializer::loadScene(const std::string& path)
{
    YAML::Node root = YAML::LoadFile(path);
    auto sceneNode = root["Scene"];

    Reference<Scene> scene = Scene::create();

    for (YAML::const_iterator it = sceneNode["Shaders"].begin(); it != sceneNode["Shaders"].end(); it++)
    {
        Reference<Shader> shader = Shader::createFromFile(sceneNode["Shaders"][it->first].as<std::string>());
        Assets::add<Shader>(it->first.as<std::string>(), shader);
        // TODO: possible add support for macros and saving shaders generated by source
    }

    for (YAML::const_iterator it = sceneNode["Texture2Ds"].begin(); it != sceneNode["Texture2Ds"].end(); it++)
    {
        Reference<Texture2D> texture;

        if (sceneNode["Texture2Ds"][it->first].as<std::string>() == "white_texture")
        {
            texture = Texture2D::createWhiteTexture();
        }
        else
        {
            texture = Texture2D::create(sceneNode["Texture2Ds"][it->first].as<std::string>());
        }

        Assets::add<Texture2D>(it->first.as<std::string>(), texture);
    }

    for (YAML::const_iterator it = sceneNode["Materials"].begin(); it != sceneNode["Materials"].end(); it++)
    {
        auto node = sceneNode["Materials"][it->first];

        auto shader = Assets::get<Shader>(node["Shader"].as<std::string>());
        Reference<Material> material = Material::create(shader);

        // Make sure they aren't invalid texture
        if (node["Albedo"].as<std::string>() != "")
            material->albedoMap = Texture2D::create(node["Albedo"].as<std::string>());
        
        if (node["Normal"].as<std::string>() != "")
            material->normalMap = Texture2D::create(node["Normal"].as<std::string>());

        if (node["Metallic"].as<std::string>() != "")
            material->metallicMap = Texture2D::create(node["Metallic"].as<std::string>());

        if (node["Roughness"].as<std::string>() != "")
            material->roughnessMap = Texture2D::create(node["Roughness"].as<std::string>());

        if (node["Ambient Occlusion"].as<std::string>() != "")
            material->ambientOcclusionMap = Texture2D::create(node["Ambient Occlusion"].as<std::string>());

        if (node["Depth"].as<std::string>() != "")
            material->depthMap = Texture2D::create(node["Depth"].as<std::string>()); // TODO: fix depth/parallax mapping

        Assets::add<Material>(it->first.as<std::string>(), material);
    }

    auto gameObjects = sceneNode["GameObjects"];

    YAML::const_iterator it;
    for (YAML::const_iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
    {
        auto object = gameObjects[it->first];

        loadGameObject(object, scene->getRootGameObject(), scene, it->first.as<std::string>());
    }
    
    return scene;
}

void SceneSerializer::loadGameObject(YAML::Node& node, GameObject& parent, const Reference<Scene>& scene, const std::string& name)
{
    auto object = parent.createChild();
    object->createComponent<Tag>(name);

    if (node["Transform"])
    {
        auto& transform = object->createComponent<Transform>();

        transform.translation.x = node["Transform"]["Translation"][0].as<float>();
        transform.translation.y = node["Transform"]["Translation"][1].as<float>();
        transform.translation.z = node["Transform"]["Translation"][2].as<float>();

        transform.rotation.x = node["Transform"]["Rotation"][0].as<float>();
        transform.rotation.y = node["Transform"]["Rotation"][1].as<float>();
        transform.rotation.z = node["Transform"]["Rotation"][2].as<float>();

        transform.scale.x = node["Transform"]["Scale"][0].as<float>();
        transform.scale.y = node["Transform"]["Scale"][1].as<float>();
        transform.scale.z = node["Transform"]["Scale"][2].as<float>();
    }

    if (node["Camera"])
    {
        auto& camera = object->createComponent<SceneCamera>();

        camera.setProjectionType(static_cast<Camera::ProjectionType>(node["Camera"]["Projection Type"].as<uint32_t>()));

        camera.setOrthoSize(node["Camera"]["Ortho Size"].as<float>());
        camera.setOrthoNear(node["Camera"]["Ortho Near"].as<float>());
        camera.setOrthoFar(node["Camera"]["Ortho Far"].as<float>());

        camera.setPerspectiveFov(node["Camera"]["Perspective Fov"].as<float>());
        camera.setPerspectiveNear(node["Camera"]["Perspective Near"].as<float>());
        camera.setPerspectiveFar(node["Camera"]["Perspective Far"].as<float>());

        camera.primary = node["Camera"]["Primary"].as<bool>();
    }

    if (node["Sprite Renderer"])
    {
        auto& spriteRenderer = object->createComponent<SpriteRendererComponent>();

        spriteRenderer.color.r = node["Sprite Renderer"]["Color"][0].as<float>();
        spriteRenderer.color.g = node["Sprite Renderer"]["Color"][1].as<float>();
        spriteRenderer.color.b = node["Sprite Renderer"]["Color"][2].as<float>();
        spriteRenderer.color.a = node["Sprite Renderer"]["Color"][3].as<float>();

        if (node["Sprite Renderer"]["Texture"].as<std::string>() != "none")
        {
            spriteRenderer.texture = Texture2D::create(node["Sprite Renderer"]["Texture"].as<std::string>());
        }

        spriteRenderer.usingTexRect = node["Sprite Renderer"]["Using Texture Rect"].as<bool>();

        spriteRenderer.textureRect.x = node["Sprite Renderer"]["Texture Rect"][0].as<float>();
        spriteRenderer.textureRect.y = node["Sprite Renderer"]["Texture Rect"][1].as<float>();
        spriteRenderer.textureRect.w = node["Sprite Renderer"]["Texture Rect"][2].as<float>();
        spriteRenderer.textureRect.h = node["Sprite Renderer"]["Texture Rect"][3].as<float>();
    }

    if (node["Mesh"])
    {
        auto& mesh = object->createComponent<MeshComponent>();

        mesh.filePath = node["Mesh"]["Mesh"].as<std::string>();
        mesh.meshID = node["Mesh"]["Mesh ID"].as<uint32_t>();
        
        bool needToLoad = true;
        if (Assets::cacheExists<Model>())
        {
            for (auto& model : Assets::getCache<Model>())
            {
                if (model.second->path == mesh.filePath)
                {
                    mesh.mesh = model.second->meshes[mesh.meshID];
                    needToLoad = false;
                    break;
                }
            }
        }

        if (needToLoad)
        {
            auto model = Model::load(mesh.filePath);
            model->path = mesh.filePath;
            Assets::add<Model>(mesh.filePath, model);
        }
    }

    if (node["Mesh Renderer"])
    {
        auto& meshRenderer = object->createComponent<MeshRendererComponent>();

        auto mat = node["Mesh Renderer"]["Material"];
        meshRenderer.material = Assets::get<Material>(mat.as<std::string>());
    }

    if (node["Directional Light"])
    {
        auto& light = object->createComponent<DirectionalLightComponent>();

        light.light.radiance.r = node["Directional Light"]["Radiance"][0].as<float>();
        light.light.radiance.g = node["Directional Light"]["Radiance"][1].as<float>();
        light.light.radiance.b = node["Directional Light"]["Radiance"][2].as<float>();

        light.light.intensity = node["Directional Light"]["Intensity"].as<float>();
    }

    if (node["Sky Light"])
    {
        auto& light = object->createComponent<SkyLightComponent>();

        light.light.intensity = node["Sky Light"]["Intensity"].as<float>();

        light.light.radiance.r = node["Sky Light"]["Radiance"][0].as<float>();
        light.light.radiance.g = node["Sky Light"]["Radiance"][1].as<float>();
        light.light.radiance.b = node["Sky Light"]["Radiance"][2].as<float>();
    }

    if (node["Point Light"])
    {
        auto& light = object->createComponent<PointLightComponent>();

        light.light.radiance.r = node["Point Light"]["Radiance"][0].as<float>();
        light.light.radiance.g = node["Point Light"]["Radiance"][1].as<float>();
        light.light.radiance.b = node["Point Light"]["Radiance"][2].as<float>();

        light.light.intensity = node["Point Light"]["Intensity"].as<float>();
    }

    if (node["Children"])
    {
        auto childrenNode = node["Children"];
        loadChildRecurse(childrenNode, *object, scene, name);
    }
}

void SceneSerializer::loadChildRecurse(YAML::Node& node, GameObject& parent, const Reference<Scene>& scene, const std::string& name)
{
    for (YAML::const_iterator it = node.begin(); it != node.end(); it++)
    {
        YAML::Node nextNode = node[it->first];
        loadGameObject(nextNode, parent, scene, it->first.as<std::string>());
    }
}

void SceneSerializer::saveGameObject(GameObject& object, YAML::Node& node)
{
    if (object.hasComponent<Transform>())
    {
        auto tc = object.getComponent<Transform>();
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

    if (object.hasComponent<SceneCamera>())
    {
        auto comp = object.getComponent<SceneCamera>();
        auto camera = node["Camera"];

        camera["Projection Type"] = static_cast<uint32_t>(comp.getProjectionType());

        camera["Ortho Size"] = comp.getOrthoSize();
        camera["Ortho Near"] = comp.getOrthoNear();
        camera["Ortho Far"] = comp.getOrthoFar();

        camera["Perspective Fov"] = comp.getPerspectiveFov();
        camera["Perspective Near"] = comp.getPerspectiveNear();
        camera["Perspective Far"] = comp.getPerspectiveFar();

        camera["Aspect"] = comp.getAspect();

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
            spriteRenderer["Texture"] = comp.texture->getPath();
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
    
        auto& name = Assets::find<Material>(comp.material);

        meshRenderer["Material"] = name;
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

    auto childrenNode = node["Children"];
    saveChildRecurse(object, childrenNode);
}

void SceneSerializer::saveChildRecurse(GameObject& parent, YAML::Node& node)
{
    for (auto& child : parent.getChildren())
    {
        auto nextNode = node[child.getComponent<Tag>().tag];
        saveGameObject(child, nextNode);
    }
}

}*/