#include "renderer/ShaderLibrary.h"

std::unordered_map<std::string, Shader> ShaderLibrary::m_shaders;

void ShaderLibrary::add(const std::string& name, Shader& shader)
{
    m_shaders.insert(std::make_pair(name, std::move(shader)));
}

void ShaderLibrary::remove(const std::string& name)
{
    m_shaders.erase(name);
}

Shader& ShaderLibrary::get(const std::string& name)
{
    return m_shaders.at(name);
}