#include <scene/SceneSerializer.h>
#include <scene/SceneEntity.h>
#include <scene/Components.h>
#include <renderer/Model.h>
#include <renderer/Assets.h>
#include <util/Timer.h>

#include <fstream>

void SceneSerializer::saveScene(const Shared<Scene>& scene, const std::string& path)
{
    YAML::Node root;
    
    auto objects = root["Scene"]["GameObjects"];

    scene->getRegistry().each([&](Entity* entityID)
    {
        SceneEntity entity(entityID, scene.get());

        auto object = objects[entity.getComponent<TagComponent>().tag];

        saveGameObject(entity, object);
    });

    std::ofstream fout(path);
    fout << root;
}

Shared<Scene> SceneSerializer::loadScene(const std::string& path)
{
    YAML::Node root = YAML::LoadFile(path);
    auto sceneNode = root["Scene"];

    Shared<Scene> scene = createShared<Scene>();

    auto gameObjects = sceneNode["GameObjects"];

    YAML::const_iterator it;
    for (YAML::const_iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
    {
        auto object = gameObjects[it->first];

        loadGameObject(object, scene->getRegistry(), scene, it->first.as<std::string>());
    }
    
    return scene;
}

#define TEXTURE_CHECK(asset, path)\
bool needToLoad = true;\
for (auto& ass : Assets::getList<Texture2D>()->getInternalList())\
{\
    if (ass.second->getPath == path)\
    {\
        asset = Assets::get<Texture2D>(ass.first);\
        needToLoad = false;\
        break;\
    }\
}\
if (needToLoad)\
{\
    asset = Texture2D::create(path);\
    Assets::add<Texture2D>(path, asset);\
}\

SceneEntity SceneSerializer::loadGameObject(YAML::Node& node, EntityRegistry& registry, const Shared<Scene>& scene, const std::string& name)
{
    auto entity = registry.create();
    registry.emplace<TagComponent>(entity, name);
    SceneEntity object(entity, scene.get());

    if (node["Transform"])
    {
        auto& transform = object.addComponent<TransformComponent>();

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
        auto& camera = object.addComponent<CameraComponent>();

        camera.camera.setProjectionType(static_cast<ProjectionType>(node["Camera"]["Projection Type"].as<uint32_t>()));

        camera.camera.setOrthoSize(node["Camera"]["Ortho Size"].as<float>());
        camera.camera.setOrthoNear(node["Camera"]["Ortho Near"].as<float>());
        camera.camera.setOrthoFar(node["Camera"]["Ortho Far"].as<float>());

        camera.camera.setPerspectiveFov(node["Camera"]["Perspective Fov"].as<float>());
        camera.camera.setPerspectiveNear(node["Camera"]["Perspective Near"].as<float>());
        camera.camera.setPerspectiveFar(node["Camera"]["Perspective Far"].as<float>());

        camera.primary = node["Camera"]["Primary"].as<bool>();
    }

    if (node["Sprite Renderer"])
    {
        auto& spriteRenderer = object.addComponent<SpriteRendererComponent>();

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
        auto& mesh = object.addComponent<MeshComponent>();

        mesh.filePath = node["Mesh"]["Mesh"].as<std::string>();
        mesh.meshID = node["Mesh"]["Mesh ID"].as<uint32_t>();
        
        bool needToLoad = true;
        if (Assets::listExists<Model>())
        {
            for (auto& model : Assets::getList<Model>()->getInternalList())
            {
                if (model.second->path == mesh.filePath)
                {
                    mesh.mesh = model.second->meshes[mesh.meshID];// TODO: add ID
                    needToLoad = false;
                    break;
                }
            }
        }

        if (needToLoad)
        {
            //auto model = Model::loadModel(mesh.filePath); // TODO: refactor model loading in some way, especially single mesh loading
            //model->path = mesh.filePath;
            //Assets::add<Model>(mesh.filePath, model);
            Shared<Mesh> meshObject = Model::loadMeshAtID(mesh.filePath, mesh.meshID);
            mesh.mesh = meshObject;
        }
    }

    if (node["Mesh Renderer"])
    {
        auto& meshRenderer = object.addComponent<MeshRendererComponent>();

        for (int i = 0; i < node["Mesh Renderer"]["Material Count"].as<int>(); i++)
        {
            auto meshRendererNode = node["Mesh Renderer"]["Materials"][i];
            //Shared<Material> material = Material::create(Shader::createFromFile(meshRendererNode["Shader"].as<std::string>()));
            Shared<Material> material = Material::create(); // TODO: sort out this situation
            //TODO: better Assets class integration
            
            //material->albedoMap = Texture2D::create(meshRendererNode["Albedo"].as<std::string>());
            //material->normalMap = Texture2D::create(meshRendererNode["Normal"].as<std::string>());
            //material->metalnessMap = Texture2D::create(meshRendererNode["Metallic"].as<std::string>());
            //material->roughnessMap = Texture2D::create(meshRendererNode["Roughness"].as<std::string>());
            //material->ambientOcclusionMap = Texture2D::create(meshRendererNode["Ambient Occlusion"].as<std::string>());
            //material->depthMap = Texture2D::create(meshRendererNode["Depth"].as<std::string>());

            material->usingAlbedoMap = meshRendererNode["Using Albedo"].as<bool>();
            material->usingNormalMap = meshRendererNode["Using Normal"].as<bool>();
            material->usingMetalnessMap = meshRendererNode["Using Metallic"].as<bool>();
            material->usingRoughnessMap = meshRendererNode["Using Roughness"].as<bool>();
            material->usingAmbientOcclusionMap = meshRendererNode["Using Ambient Occlusion"].as<bool>();
            material->usingDepthMap = meshRendererNode["Using Depth"].as<bool>();

            meshRenderer.materials.push_back(material);
        }
    }

    if (node["Directional Light"]) // TODO: add directional lights pbr
    {
        auto& light = object.addComponent<DirectionalLightComponent>();

        light.radiance.r = node["Directional Light"]["Radiance"][0].as<float>();
        light.radiance.g = node["Directional Light"]["Radiance"][1].as<float>();
        light.radiance.b = node["Directional Light"]["Radiance"][2].as<float>();

        light.intensity = node["Directional Light"]["Intensity"].as<float>();
    }

    if (node["Sky Light"])
    {
        auto& light = object.addComponent<SkyLightComponent>();

        light.intensity = node["Sky Light"]["Intensity"].as<float>();
    }

    if (node["Point Light"])
    {
        auto& light = object.addComponent<PointLightComponent>();

        light.radiance.r = node["Point Light"]["Radiance"][0].as<float>();
        light.radiance.g = node["Point Light"]["Radiance"][1].as<float>();
        light.radiance.b = node["Point Light"]["Radiance"][2].as<float>();

        light.intensity = node["Point Light"]["Intensity"].as<float>();
        light.attenuation = node["Point Light"]["Attenuation"].as<float>();
    }

    if (node["Children"])
    {
        auto childrenNode = node["Children"];
        loadChildRecurse(childrenNode, *entity->getChildren(), scene, name);
    }
}

void SceneSerializer::loadChildRecurse(YAML::Node& node, EntityRegistry& registry, const Shared<Scene>& scene, const std::string& name)
{
    for (YAML::const_iterator it = node.begin(); it != node.end(); it++)
    {
        YAML::Node nextNode = node[it->first];
        loadGameObject(nextNode, registry, scene, name);
    }
}

void SceneSerializer::saveGameObject(SceneEntity& entity, YAML::Node& node)
{
    if (entity.hasComponent<TransformComponent>())
    {
        auto tc = entity.getComponent<TransformComponent>();
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

    if (entity.hasComponent<CameraComponent>())
    {
        auto comp = entity.getComponent<CameraComponent>();
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

    if (entity.hasComponent<SpriteRendererComponent>())
    {
        auto comp = entity.getComponent<SpriteRendererComponent>();

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

    if (entity.hasComponent<MeshComponent>())
    {
        auto comp = entity.getComponent<MeshComponent>();

        auto mesh = node["Mesh"];

        mesh["Mesh"] = comp.filePath;
        mesh["Mesh ID"] = comp.meshID;
    }

    if (entity.hasComponent<MeshRendererComponent>())
    {
        auto comp = entity.getComponent<MeshRendererComponent>();

        auto meshRenderer = node["Mesh Renderer"];

        meshRenderer["Material Count"] = comp.materials.size();
        
        int i = 0;
        for (auto& material : comp.materials)
        {
            meshRenderer["Materials"][i]["Albedo"] = material->albedoMap->getPath();
            meshRenderer["Materials"][i]["Using Albedo"] = material->usingAlbedoMap;

            meshRenderer["Materials"][i]["Normal"] = material->normalMap->getPath();
            meshRenderer["Materials"][i]["Using Normal"] = material->usingNormalMap;

            meshRenderer["Materials"][i]["Metallic"] = material->metalnessMap->getPath();
            meshRenderer["Materials"][i]["Using Metallic"] = material->usingMetalnessMap;
            
            meshRenderer["Materials"][i]["Roughness"] = material->roughnessMap->getPath();
            meshRenderer["Materials"][i]["Using Roughness"] = material->usingRoughnessMap;

            meshRenderer["Materials"][i]["Ambient Occlusion"] = material->ambientOcclusionMap->getPath();
            meshRenderer["Materials"][i]["Using Ambient Occlusion"] = material->usingAmbientOcclusionMap;

            meshRenderer["Materials"][i]["Depth"] = material->depthMap->getPath();
            meshRenderer["Materials"][i]["Using Depth"] = material->usingDepthMap;

            meshRenderer["Materials"][i]["Shader"] = material->shader->getPath();

            i++;
        }
    }

    if (entity.hasComponent<DirectionalLightComponent>())
    {
        auto comp = entity.getComponent<DirectionalLightComponent>();

        auto light = node["Directional Light"];

        light["Radiance"][0] = comp.radiance.r;
        light["Radiance"][1] = comp.radiance.g;
        light["Radiance"][2] = comp.radiance.b;

        light["Intensity"] = comp.intensity;
    }

    if (entity.hasComponent<PointLightComponent>())
    {
        auto comp = entity.getComponent<PointLightComponent>();

        auto light = node["Point Light"];

        light["Radiance"][0] = comp.radiance.r;
        light["Radiance"][1] = comp.radiance.g;
        light["Radiance"][2] = comp.radiance.b;

        light["Intensity"] = comp.intensity;
        light["Attenuation"] = comp.attenuation;
    }

    if (entity.hasComponent<SkyLightComponent>())
    {
        auto comp = entity.getComponent<SkyLightComponent>();

        auto light = node["Sky Light"];

        light["Intensity"] = comp.intensity;
    }

    auto childrenNode = node["Children"];
    saveChildRecurse(entity, childrenNode);
}

void SceneSerializer::saveChildRecurse(SceneEntity& parent, YAML::Node& node)
{
    for (auto& child : parent.getChildren())
    {
        auto nextNode = node[child.getComponent<TagComponent>().tag];
        std::cout << child.getComponent<TagComponent>().tag << "\n";
        saveGameObject(child, nextNode);
    }
}