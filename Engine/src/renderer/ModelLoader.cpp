#include <renderer/ModelLoader.h>
#include <core/Logger.h>
#include <renderer/Assets.h>

namespace Engine
{

Shared<Model> ModelLoader::load(const std::string& path)
{
    if (m_modelsLoaded.find(path) != m_modelsLoaded.end())
    {
        return m_modelsLoaded.at(path); // NOTE: Need to test, a deep copy might be necessary for specific use cases
    }

    Shared<Model> model = createShared<Model>();

    auto scene = setupAssimp_(path);

    model->path = path;
    model->directory = path.substr(0, path.find_last_of('/'));

    processNode_(scene->mRootNode, scene, model);

    m_modelsLoaded.emplace(std::make_pair(path, model));

    return model;
}

const aiScene* ModelLoader::setupAssimp_(const std::string& modelPath)
{
    const aiScene* scene = m_importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        Logger::getCoreLogger()->error("[ASSIMP] %s", m_importer.GetErrorString());
    }

    return scene;
}

Shared<Mesh> ModelLoader::loadMesh(const std::string& path, unsigned int id)
{
    auto scene = setupAssimp_(path);

    Shared<Model> temp = createShared<Model>();
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

void ModelLoader::processNode_(aiNode* node, const aiScene* scene, const Shared<Model>& model)
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

Shared<Mesh> ModelLoader::processMesh_(aiMesh* mesh, const aiScene* scene, const Shared<Model>& model)
{
    Shared<Material> material;
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

            auto material_ = Material::create(Assets::get<Shader>("pbr")); // TODO: material shaders

            if (!albedo)
                material_->usingAlbedoMap = false;
            else
            {
                material_->albedoMap = albedo;
                material_->usingAlbedoMap = true;
            }

            if (!normal)
                material_->usingNormalMap = false;
            else
            {
                material_->normalMap = normal;
                material_->usingNormalMap = true;
            }

            if (!metallic)
                material_->usingMetallicMap = false;
            else
            {
                material_->metallicMap = metallic;
                material_->usingMetallicMap = true;
            }

            if (!roughness)
            {
                material_->usingRoughnessMap = false;
                material_->roughnessScalar = 1.f;
            }
            else
            {
                material_->roughnessMap = roughness;
                material_->usingRoughnessMap = true;
            }

            if (!ambientOcclusion)
                material_->usingAmbientOcclusionMap = false;
            else
            {
                material_->ambientOcclusionMap = ambientOcclusion;
                material_->usingAmbientOcclusionMap = true;
            }

            material = material_;
            m_materialsLoaded.emplace(std::pair<int, Shared<Material>>(mesh->mMaterialIndex, material));
        }
    //}

    Shared<Mesh> mesh_ = createShared<Mesh>();
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

Shared<Texture2D> ModelLoader::loadMaterialTexture_(aiMaterial* mat, aiTextureType type, const std::string& directory)
{
    Shared<Texture2D> texture = nullptr;

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
        texture = Texture2D::create(directory + "/" + str.C_Str(), GL_SRGB8_ALPHA8);

        m_texturesLoaded.push_back(texture);
    }

    return texture;
}
}