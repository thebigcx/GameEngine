#pragma once

#include <yaml-cpp/yaml.h>

#include <scene/Scene.h>

class SceneSerializer
{
public:
    static void saveScene(const Shared<Scene>& scene, const std::string& path);
    static Shared<Scene> loadScene(const std::string& path);

private:
    static SceneEntity loadGameObject(YAML::Node& node, const Shared<Scene>& scene, const std::string& name);
    static void saveGameObject(SceneEntity& entity, YAML::Node& node);
};