#include <renderer/Mesh.h>
#include <renderer/Model.h>
#include <renderer/ModelLoader.h>
#include <renderer/MeshFactory.h>

namespace Engine
{

Shared<Mesh> Mesh::load(const std::string& path, unsigned int id)
{
    return ModelLoader::loadMesh(path, id);
}

void Mesh::setVertices(const std::vector<math::vec3>& positions, const std::vector<math::vec3>& uvs, const std::vector<uint32_t>& indices)
{
    vertexArray->bind();
    std::vector<ModelVertex> vertices;

    for (unsigned int i = 0; i < positions.size(); i++)
    {
        ModelVertex vertex;
        vertex.position = positions[i];
        vertex.uv = uvs[i];
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < indices.size(); i += 3)
    {
        math::vec3 normal = MeshFactory::calculateNormal(vertices[indices[i    ]].position,
                                                         vertices[indices[i + 1]].position,
                                                         vertices[indices[i + 2]].position);

        math::vec3 tangent = MeshFactory::calculateTangent(vertices[indices[i    ]].position,
                                                           vertices[indices[i + 2]].position,
                                                           vertices[indices[i + 2]].position,
                                                           vertices[indices[i    ]].uv,
                                                           vertices[indices[i + 1]].uv,
                                                           vertices[indices[i + 2]].uv);

        vertices[indices[i    ]].normal = normal;
        vertices[indices[i + 1]].normal = normal;
        vertices[indices[i + 2]].normal = normal;

        vertices[indices[i    ]].tangent = tangent;
        vertices[indices[i + 1]].tangent = tangent;
        vertices[indices[i + 2]].tangent = tangent;
    }

    if (vertices.size() == 0)
    {
        
    }
    else
    {

    }

    if (indices.size() == 0)
    {

    }
    else
    {

    }

    vertexBuffer->setData(&vertices[0], vertices.size() * sizeof(ModelVertex));
    indexBuffer->setData(&indices[0], indices.size());
}

}