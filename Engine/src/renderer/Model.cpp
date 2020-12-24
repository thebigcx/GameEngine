#include <renderer/Model.h>
#include <renderer/Renderer3D.h>
#include <renderer/MeshFactory.h>
#include <renderer/Assets.h>
#include <util/Timer.h>
#include <renderer/Assets.h>

Shared<Model> Model::loadModel(const std::string& file)
{
    auto model = createShared<Model>();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        Logger::getCoreLogger()->error("[ASSIMP] %s", importer.GetErrorString());
        return model;
    }

    model->path = file;
    model->m_directory = file.substr(0, file.find_last_of('/'));

    model->processNode(scene->mRootNode, scene, model);

    return model;
}

Shared<Mesh> Model::loadMeshAtID(const std::string& file, unsigned int id)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        Logger::getCoreLogger()->error("[ASSIMP] %s", importer.GetErrorString());
    }

    Shared<Model> temp = createShared<Model>();
    temp->path = file;
    temp->m_directory = file.substr(0, file.find_last_of('/'));

    if (id > scene->mNumMeshes)
    {
        Logger::getCoreLogger()->error("Mesh ID (%i) greater than amount of meshes!", id);
    }

    aiMesh* mesh = scene->mMeshes[id];

    return temp->processMesh(mesh, scene, temp);
}

void Model::processNode(aiNode* node, const aiScene* scene, const Shared<Model>& model)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, model));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, model);
    }
}

Shared<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene, const Shared<Model>& model)
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

        if (mesh->mNormals != nullptr)
        {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }
        else
        {
            vertex.normal = math::vec3(0.f);
        }

        if (mesh->mTextureCoords[0])
        {
            vertex.uv.x = mesh->mTextureCoords[0][i].x;
            vertex.uv.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.uv = math::vec2(0.f);
        }

        if (mesh->mTangents != nullptr)
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

    bool needToLoadMaterial = true;
    for (auto& materialLoaded : m_materialsLoaded)
    {
        if (mesh->mMaterialIndex == materialLoaded.first)
        {
            needToLoadMaterial = false;
            material = materialLoaded.second;
            break;
        }
    }

    if (needToLoadMaterial)
    {
        if (mesh->mMaterialIndex > 0)
        {
            aiMaterial* aimaterial = scene->mMaterials[mesh->mMaterialIndex];

            auto albedo = loadMaterialTexture(aimaterial, aiTextureType_DIFFUSE);
            auto normal = loadMaterialTexture(aimaterial, aiTextureType_HEIGHT);
            auto metallic = loadMaterialTexture(aimaterial, aiTextureType_REFLECTION);
            auto roughness = loadMaterialTexture(aimaterial, aiTextureType_SHININESS);
            auto ambientOcclusion = loadMaterialTexture(aimaterial, aiTextureType_AMBIENT);

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
                material_->usingRoughnessMap = false;
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

            std::string name = std::string("material_") + std::to_string(Assets::getAssetCount<Material>());
            Assets::add<Material>(name, material);
        }
    }

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

Shared<Texture2D> Model::loadMaterialTexture(aiMaterial* mat, aiTextureType type)
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
        std::string texturePath = m_directory + "/" + std::string(str.C_Str());
        if (std::strcmp(m_texturesLoaded[j]->getPath().c_str(), texturePath.c_str()) == 0)
        {
            texture = m_texturesLoaded[j];
            skip = true;
            break;
        }
    }
    if (!skip)
    {
        texture = Texture2D::create(m_directory + "/" + str.C_Str());

        m_texturesLoaded.push_back(texture);
    }

    return texture;
}