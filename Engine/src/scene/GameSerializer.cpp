#include <scene/GameSerializer.h>
#include <renderer/Assets.h>
#include <renderer/Material.h>
#include <script/Script.h>

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace Engine
{

void GameSerializer::saveProject(const std::string& path)
{
    YAML::Node root;

    YAML::Node scenes = root["Scenes"];

    for (auto& material : Assets::getCache<Material>())
    {
        YAML::Node matNode;

        matNode["uuid"] = material.first;

        std::ofstream fout(path + "/Materials/" + material.second->name + ".material");
        fout << matNode;
    }

    std::ofstream fout(path);
    fout << root;
}

}