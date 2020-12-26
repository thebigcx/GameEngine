#include <renderer/Mesh.h>
#include <renderer/Model.h>

namespace Engine
{

Shared<Mesh> Mesh::load(const std::string& path, unsigned int id)
{
    return Model::loadMeshAtID(path, id); // TODO: refactor this. options: transfer model loading to mesh class, or create a ModelLoader class
}

}