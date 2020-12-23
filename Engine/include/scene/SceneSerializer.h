#pragma once

#include <yaml-cpp/yaml.h>

#include <scene/Scene.h>

class SceneSerializer
{
public:
    static void saveScene(const Shared<Scene>& scene, const std::string& path);
    static Shared<Scene> loadScene(const std::string& path);

private:
    static void loadGameObject(YAML::Node& node, EntityRegistry& registry, const Shared<Scene>& scene, const std::string& name);
    static void saveGameObject(SceneEntity& entity, YAML::Node& node);

    static void saveChildRecurse(SceneEntity& parent, YAML::Node& node);
    static void loadChildRecurse(YAML::Node& node, EntityRegistry& registry, const Shared<Scene>& scene, const std::string& name);
};