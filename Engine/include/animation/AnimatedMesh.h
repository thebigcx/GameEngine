#pragma once

#include <renderer/Mesh.h>

namespace Engine
{

class AnimatedMesh : public Mesh
{
public:
    AnimatedMesh() {}

    static Reference<AnimatedMesh> load(const std::string& path, unsigned int id);
};

}