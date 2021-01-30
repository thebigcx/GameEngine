#include <renderer/Model.h>
#include <renderer/ModelLoader.h>

namespace Engine
{

Reference<Model> Model::load(const std::string& file)
{
    return ModelLoader::load(file);
}

}