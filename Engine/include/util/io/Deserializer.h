#pragma once

#include <string>

#include <core/Core.h>

namespace Engine
{

class Mesh;
class Material;
class Texture2D;
class Shader;

class Deserializer
{
public:
    static Reference<Material> loadMaterial(const std::string& path);
    static Reference<Texture2D> loadTexture(const std::string& path);
    static Reference<Shader> loadShader(const std::string& path);
    static Reference<Mesh> loadMesh(const std::string& path);
};

}