#include <renderer/Model.h>
#include <renderer/Renderer3D.h>
#include <renderer/MeshFactory.h>
#include <renderer/Assets.h>
#include <util/Timer.h>
#include <renderer/Assets.h>
#include <renderer/ModelLoader.h>

namespace Engine
{

Shared<Model> Model::load(const std::string& file)
{
    return ModelLoader::load(file);
}

}