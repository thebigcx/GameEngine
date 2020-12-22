#include <scene/SceneSerializer.h>
#include <scene/SceneEntity.h>
#include <scene/Components.h>

#include <yaml-cpp/yaml.h>

#include <fstream>

void SceneSerializer::saveScene(const Shared<Scene>& scene, const std::string& path)
{
    YAML::Node root;
    
    auto objects = root["Scene"]["GameObjects"];

    scene->getRegistry().each([&](Entity* entityID)
    {
        SceneEntity entity(entityID, scene.get());

        auto object = objects[entity.getComponent<TagComponent>().tag];

        if (entity.hasComponent<TransformComponent>())
        {
            auto tc = entity.getComponent<TransformComponent>();
            auto transform = object["Transform"];

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
            auto camera = object["Camera"];

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

            auto spriteRenderer = object["Sprite Renderer"];

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
        auto entity = scene->createEntity(it->first.as<std::string>());

        if (it->second["Transform"])
        {
            auto transform = entity.addComponent<TransformComponent>();

            transform.translation.x = object["Transform"]["Translation"][0].as<float>();
            transform.translation.y = object["Transform"]["Translation"][1].as<float>();
            transform.translation.z = object["Transform"]["Translation"][2].as<float>();

            transform.rotation.x = object["Transform"]["Rotation"][0].as<float>();
            transform.rotation.y = object["Transform"]["Rotation"][1].as<float>();
            transform.rotation.z = object["Transform"]["Rotation"][2].as<float>();

            transform.scale.x = object["Transform"]["Scale"][0].as<float>();
            transform.scale.y = object["Transform"]["Scale"][1].as<float>();
            transform.scale.z = object["Transform"]["Scale"][2].as<float>();
        }

        if (it->second["Camera"])
        {
            auto camera = entity.addComponent<CameraComponent>();

            camera.camera.setProjectionType(static_cast<ProjectionType>(object["Camera"]["Projection Type"].as<uint32_t>()));

            camera.camera.setOrthoSize(object["Camera"]["Ortho Size"].as<float>());
            camera.camera.setOrthoNear(object["Camera"]["Ortho Near"].as<float>());
            camera.camera.setOrthoFar(object["Camera"]["Ortho Far"].as<float>());

            camera.camera.setPerspectiveFov(object["Camera"]["Perspective Fov"].as<float>());
            camera.camera.setPerspectiveNear(object["Camera"]["Perspective Near"].as<float>());
            camera.camera.setPerspectiveFar(object["Camera"]["Perspective Far"].as<float>());

            camera.primary = object["Camera"]["Primary"].as<bool>();
        }

        if (it->second["Sprite Renderer"])
        {
            auto spriteRenderer = entity.addComponent<SpriteRendererComponent>();

            spriteRenderer.color.r = object["Sprite Renderer"]["Color"][0].as<float>();
            spriteRenderer.color.g = object["Sprite Renderer"]["Color"][1].as<float>();
            spriteRenderer.color.b = object["Sprite Renderer"]["Color"][2].as<float>();
            spriteRenderer.color.a = object["Sprite Renderer"]["Color"][3].as<float>();

            if (object["Sprite Renderer"]["Texture"].as<std::string>() != "none")
            {
                spriteRenderer.texture = Texture2D::create(object["Sprite Renderer"]["Texture"].as<std::string>());
            }

            spriteRenderer.usingTexRect = object["Sprite Renderer"]["Using Texture Rect"].as<bool>();

            spriteRenderer.textureRect.x = object["Sprite Renderer"]["Texture Rect"][0].as<float>();
            spriteRenderer.textureRect.y = object["Sprite Renderer"]["Texture Rect"][1].as<float>();
            spriteRenderer.textureRect.w = object["Sprite Renderer"]["Texture Rect"][2].as<float>();
            spriteRenderer.textureRect.h = object["Sprite Renderer"]["Texture Rect"][3].as<float>();
        }

    }
    
    return scene;
}