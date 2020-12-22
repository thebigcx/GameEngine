#pragma once

#include <scene/Scene.h>

class SceneSerializer
{
public:
    static void saveScene(const Shared<Scene>& scene, const std::string& path);
    static Shared<Scene> loadScene(const std::string& path);
};