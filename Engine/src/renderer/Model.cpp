#include <renderer/Model.h>
#include <renderer/ModelLoader.h>

namespace Engine
{

Shared<Model> Model::load(const std::string& file)
{
    return ModelLoader::load(file);
}

}