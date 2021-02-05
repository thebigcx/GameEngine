#include <util/io/Deserializer.h>
#include <renderer/Material.h>
#include <renderer/Assets.h>
#include <renderer/Mesh.h>

#include <yaml-cpp/yaml.h>

namespace Engine
{

Reference<Material> Deserializer::loadMaterial(const std::string& path)
{
    YAML::Node meta = YAML::LoadFile(path + ".meta");
    std::string uuid = meta["uuid"].as<std::string>();

    YAML::Node root = YAML::LoadFile(path);
    auto matNode = root["Material"];

    std::string name = matNode["Name"].as<std::string>();
    std::string shaderUUID = matNode["Shader"]["uuid"].as<std::string>();

    auto material = Material::create(/* Get shader by UUID */);

    material->albedoMap = Assets::get<Texture2D>(matNode["Textures"]["Albedo"]["uuid"].as<std::string>());
    material->normalMap = Assets::get<Texture2D>(matNode["Textures"]["Normal"]["uuid"].as<std::string>());
    material->metallicMap = Assets::get<Texture2D>(matNode["Textures"]["Metallic"]["uuid"].as<std::string>());
    material->roughnessMap = Assets::get<Texture2D>(matNode["Textures"]["Roughness"]["uuid"].as<std::string>());
    material->ambientOcclusionMap = Assets::get<Texture2D>(matNode["Textures"]["Ambient Occlusion"]["uuid"].as<std::string>());
    material->emissionMap = Assets::get<Texture2D>(matNode["Textures"]["Emission"]["uuid"].as<std::string>());

    return material;
}

Reference<Texture2D> Deserializer::loadTexture(const std::string& path)
{
    YAML::Node meta = YAML::LoadFile(path + ".meta");
    std::string uuid = meta["uuid"].as<std::string>();
    bool clamp = meta["Texture"]["Clamp"].as<bool>();
    bool linear = meta["Texture"]["Linear"].as<bool>();
    bool isSRGB = meta["Texture"]["IsSRGB"].as<bool>();

    return Texture2D::create(path, clamp, linear, isSRGB);
}

Reference<Shader> Deserializer::loadShader(const std::string& path)
{
    YAML::Node meta = YAML::LoadFile(path + ".meta");

    return Shader::createFromFile(path);
}

Reference<Mesh> Deserializer::loadMesh(const std::string& path)
{
    YAML::Node meta = YAML::LoadFile(path + ".meta");

    return Mesh::load(path, 0);
}

}