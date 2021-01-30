#include <renderer/ModelLoader.h>
#include <core/Logger.h>
#include <renderer/Assets.h>

namespace Engine
{

Reference<Model> ModelLoader::load(const std::string& path)
{
    if (m_modelsLoaded.find(path) != m_modelsLoaded.end())
    {
        return m_modelsLoaded.at(path); // NOTE: Need to test, a deep copy might be necessary for specific use cases
    }

    Reference<Model> model = createReference<Model>();

    auto scene = setupAssimp_(path);

    model->path = path;
    model->directory = path.substr(0, path.find_last_of('/'));

    processNode_(scene->mRootNode, scene, model);

    m_modelsLoaded.emplace(std::make_pair(path, model));

    return model;
}

const aiScene* ModelLoader::setupAssimp_(const std::string& modelPath)
{
    const aiScene* scene = m_importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        Logger::getCoreLogger()->error("[ASSIMP] %s", m_importer.GetErrorString());
    }

    return scene;
}

Reference<Mesh> ModelLoader::loadMesh(const std::string& path, unsigned int id)
{
    auto scene = setupAssimp_(path);

    Reference<Model> temp = createReference<Model>();
    temp->path = path;
    temp->directory = path.substr(0, path.find_last_of('/'));

    if (id >= scene->mNumMeshes)
    {
        Logger::getCoreLogger()->error("Mesh ID (%i) greater than amount of meshes!", id);

        aiMesh* mesh = scene->mMeshes[0];

        return processMesh_(mesh, scene, temp);
    }

    aiMesh* mesh = scene->mMeshes[id];

    return processMesh_(mesh, scene, temp);
}

void ModelLoader::processNode_(aiNode* node, const aiScene* scene, const Reference<Model>& model)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model->meshes.push_back(processMesh_(mesh, scene, model));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode_(node->mChildren[i], scene, model);
    }
}

Reference<Mesh> ModelLoader::processMesh_(aiMesh* mesh, const aiScene* scene, const Reference<Model>& model)
{
    Reference<Material> material;
    std::vector<ModelVertex> vertices;
    std::vector<uint32_t> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        ModelVertex vertex;

        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        if (mesh->mNormals)
        {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }
        else
        {
            vertex.normal = math::vec3(0.f);
        }

        if (mesh->mTextureCoords)
        {
            vertex.uv.x = mesh->mTextureCoords[0][i].x;
            vertex.uv.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.uv = math::vec2(0.f);
        }

        if (mesh->mTangents)
        {
            vertex.tangent.x = mesh->mTangents[i].x;
            vertex.tangent.y = mesh->mTangents[i].y;
            vertex.tangent.z = mesh->mTangents[i].z;
        }
        else
        {
            vertex.tangent = math::vec3(0.f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
/*
    bool needToLoadMaterial = true;
    for (auto& materialLoaded : m_materialsLoaded)
    {
        if (mesh->mMaterialIndex == materialLoaded.first) // FIXME: doesn't work
        {
            needToLoadMaterial = false;
            material = materialLoaded.second;
            break;
        }
    }

    if (needToLoadMaterial)
    {*/
        if (mesh->mMaterialIndex > 0)
        {
            aiMaterial* aimaterial = scene->mMaterials[mesh->mMaterialIndex];

            auto albedo = loadMaterialTexture_(aimaterial, aiTextureType_DIFFUSE, model->directory);
            auto normal = loadMaterialTexture_(aimaterial, aiTextureType_NORMALS, model->directory);
            auto metallic = loadMaterialTexture_(aimaterial, aiTextureType_REFLECTION, model->directory);
            auto roughness = loadMaterialTexture_(aimaterial, aiTextureType_SHININESS, model->directory);
            auto ambientOcclusion = loadMaterialTexture_(aimaterial, aiTextureType_LIGHTMAP, model->directory);
            auto emission = loadMaterialTexture_(aimaterial, aiTextureType_EMISSIVE, model->directory);

            auto material_ = Material::create(Assets::get<Shader>("pbr")); // TODO: material shaders

            material->albedoMap = albedo;
            material->normalMap = normal;
            material->metallicMap = metallic;
            material->roughnessMap = roughness;
            material->ambientOcclusionMap = ambientOcclusion;
            material->emissionMap = emission;

            material = material_;
            m_materialsLoaded.emplace(std::pair<int, Reference<Material>>(mesh->mMaterialIndex, material));
        }
    //}

    Reference<Mesh> mesh_ = createReference<Mesh>();
    mesh_->vertexArray = VertexArray::create();
    mesh_->vertexArray->bind();

    BufferLayout layout = {
        { Shader::DataType::Float3, "aPos"      },
        { Shader::DataType::Float3, "aNormal"   },
        { Shader::DataType::Float2, "aTexCoord" },
        { Shader::DataType::Float3, "aTangent"  }
    };

    mesh_->indexBuffer = IndexBuffer::create(indices.size(), IndexDataType::UInt32);
    mesh_->indexBuffer->setData(&indices[0], indices.size());
    
    mesh_->vertexBuffer = VertexBuffer::create(vertices.size() * sizeof(ModelVertex));
    mesh_->vertexBuffer->setData(&vertices[0], vertices.size() * sizeof(ModelVertex));
    mesh_->vertexBuffer->setLayout(layout);
    
    mesh_->vertexArray->addVertexBuffer(mesh_->vertexBuffer);
    mesh_->vertexArray->setIndexBuffer(mesh_->indexBuffer);

    mesh_->material = material;

    return mesh_;
}

Reference<Texture2D> ModelLoader::loadMaterialTexture_(aiMaterial* mat, aiTextureType type, const std::string& directory)
{
    Reference<Texture2D> texture = nullptr;

    if (mat->GetTextureCount(type) == 0)
    {
        return texture;
    }

    aiString str;
    if (mat->GetTexture(type, 0, &str) == AI_FAILURE)
    {
        Logger::getCoreLogger()->error("[ASSIMP] Could not open texture material!");
    }

    bool skip = false;
    for (unsigned int j = 0; j < m_texturesLoaded.size(); j++)
    {
        std::string texturePath = directory + "/" + std::string(str.C_Str());
        if (std::strcmp(m_texturesLoaded[j]->getPath().c_str(), texturePath.c_str()) == 0)
        {
            texture = m_texturesLoaded[j];
            skip = true;
            break;
        }
    }
    if (!skip)
    {
        texture = Texture2D::create(directory + "/" + str.C_Str());

        m_texturesLoaded.push_back(texture);
    }

    return texture;
}
}