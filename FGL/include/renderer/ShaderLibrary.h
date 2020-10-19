#pragma once

#include <string>

#include "Shader.h"

class ShaderLibrary
{
public:
    // Moves shader into m_shaders (after add() is called, previous shader cannot be used)
    static void add(const std::string& name, Shader& shader);
    static void remove(const std::string& name);

    static Shader& get(const std::string& name);

private:
    static std::unordered_map<std::string, Shader> m_shaders;
};