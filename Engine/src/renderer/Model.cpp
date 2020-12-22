#include <renderer/Model.h>
#include <renderer/Renderer3D.h>
#include <renderer/MeshFactory.h>
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

    model->m_directory = file.substr(0, file.find_last_of('/'));

    model->processNode(scene->mRootNode, scene);

    return model;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Shared<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Shared<Material>> materials;
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


    if (mesh->mMaterialIndex > 0)
    {
        aiMaterial* aimaterial = scene->mMaterials[mesh->mMaterialIndex];

        auto albedos = loadMaterialTextures(aimaterial, aiTextureType_DIFFUSE);
        auto normals = loadMaterialTextures(aimaterial, aiTextureType_HEIGHT);
        auto metalnesses = loadMaterialTextures(aimaterial, aiTextureType_REFLECTION);
        auto roughnesses = loadMaterialTextures(aimaterial, aiTextureType_SHININESS);
        auto aos = loadMaterialTextures(aimaterial, aiTextureType_AMBIENT);

        for (size_t i = 0; i < albedos.size(); i++)
        {
            auto material = Material::create(Assets::get<Shader>("pbr")); // TODO: material shaders

            if (i >= albedos.size())
                material->usingAlbedoMap = false;
            else
            {
                material->albedoMap = albedos.at(i);
                material->usingAlbedoMap = true;
            }

            if (i >= normals.size())
                material->usingNormalMap = false;
            else
            {
                material->normalMap = normals.at(i);
                material->usingNormalMap = true;
            }

            if (i >= metalnesses.size())
                material->usingMetalnessMap = false;
            else
            {
                material->metalnessMap = metalnesses.at(i);
                material->usingMetalnessMap = true;
            }

            if (i >= roughnesses.size())
                material->usingRoughnessMap = false;
            else
            {
                material->roughnessMap = roughnesses.at(i);
                material->usingRoughnessMap = true;
            }

            if (i >= aos.size())
                material->usingAmbientOcclusionMap = false;
            else
            {
                material->ambientOcclusionMap = aos.at(i);
                material->usingAmbientOcclusionMap = true;
            }
            // TODO: use Assets<Material> instead
            for (auto& materialLoaded : Assets::getList<Material>()->getInternalList())
            {
                if (*materialLoaded.second == *material)
                {
                    material = materialLoaded.second;
                }
            }
            materials.push_back(material);
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

    mesh_->materials = materials;

    return mesh_;
}

std::vector<Shared<Texture2D>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
    std::vector<Shared<Texture2D>> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        if (mat->GetTexture(type, i, &str) == AI_FAILURE)
        {
            Logger::getCoreLogger()->error("[ASSIMP] Could not open texture material!");
        }

        bool skip = false;
        for (unsigned int j = 0; j < m_texturesLoaded.size(); j++)
        {
            std::string texture = m_directory + "/" + std::string(str.C_Str());
            if (std::strcmp(m_texturesLoaded[j]->getPath().c_str(), texture.c_str()) == 0)
            {
                textures.push_back(m_texturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Shared<Texture2D> texture;
            texture = Texture2D::create(m_directory + "/" + str.C_Str());
            textures.push_back(texture);

            m_texturesLoaded.push_back(texture);
        }
    }

    return textures;
}