#pragma once

#include <core/Core.h>
#include <renderer/Material.h>

#include <yaml-cpp/yaml.h>

namespace Engine
{

class Mesh;
class Scene;
class Script;
class GameObject;

class Serializer
{
public:
    static void saveMaterial(const Reference<Material>& material, const std::string& path);
    static void saveTexture(const Reference<Texture2D>& texture);
    static void saveShader(const Reference<Shader>& shader);
    static void saveMesh(const Reference<Mesh>& mesh);
    static void saveScene(const Reference<Scene>& scene, const std::string& path);
    static void saveScript(const Reference<Script>& script);
    static void saveGameObject(YAML::Node& node, GameObject& object);
};

}