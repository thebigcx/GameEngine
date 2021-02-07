#pragma once

#include <string>

#include <core/Core.h>
#include <yaml-cpp/yaml.h>

namespace Engine
{

class Mesh;
class Material;
class Texture2D;
class Shader;
class Scene;
class GameObject;

class Deserializer
{
public:
    static Reference<Material> loadMaterial(const std::string& path);
    static Reference<Texture2D> loadTexture(const std::string& path);
    static Reference<Shader> loadShader(const std::string& path);
    static Reference<Mesh> loadMesh(const std::string& path);
    static Reference<Scene> loadScene(const std::string& path);

    static void loadGameObject(GameObject& parent, YAML::Node& node);
};

}