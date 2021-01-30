#pragma once

#include <yaml-cpp/yaml.h>

#include <scene/Scene.h>

namespace Engine
{

class SceneSerializer
{
public:
    static void saveScene(const Reference<Scene>& scene, const std::string& path);
    static Reference<Scene> loadScene(const std::string& path);

private:
    static void loadGameObject(YAML::Node& node, GameObject& parent, const Reference<Scene>& scene, const std::string& name);
    static void saveGameObject(GameObject& object, YAML::Node& node);

    static void saveChildRecurse(GameObject& parent, YAML::Node& node);
    static void loadChildRecurse(YAML::Node& node, GameObject& registry, const Reference<Scene>& scene, const std::string& name);
};

}