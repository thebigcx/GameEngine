#include <renderer/Model.h>
#include <renderer/Renderer3D.h>
#include <renderer/MeshFactory.h>
#include <renderer/Assets.h>

Shared<Model> Model::loadModel(const std::string& file)
{
    auto model = createShared<Model>();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);

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

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0])
        {
            vertex.uv.x = mesh->mTextureCoords[0][i].x;
            vertex.uv.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.uv = math::vec2(0.f);
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
        Shared<Material> material = Material::create(Renderer3D::data.modelShader); // TODO: material shaders

        auto albedos = loadMaterialTextures(aimaterial, aiTextureType_DIFFUSE);
        if (albedos.size() > 0)
        {
            material->albedoMap = albedos[0];
        }

        auto normals = loadMaterialTextures(aimaterial, aiTextureType_NORMALS);
        if (normals.size() > 0)
        {
            material->normalMap = normals[0];
        }

        auto metalnesses = loadMaterialTextures(aimaterial, aiTextureType_REFLECTION);
        if (metalnesses.size() > 0)
        {
            material->metalnessMap = metalnesses[0];
        }

        //material->metalness = 0.f;

        auto roughnesses = loadMaterialTextures(aimaterial, aiTextureType_SHININESS);
        if (roughnesses.size() > 0)
        {
            material->roughnessMap = roughnesses[0];
        }

        //material->roughness = roughness;

        materials.push_back(material);
    }

    Shared<Mesh> mesh_ = createShared<Mesh>();
    mesh_->vertexArray = VertexArray::create();
    mesh_->vertexArray->bind();

    BufferLayout layout = {
        { Shader::DataType::Float3, "aPos"      },
        { Shader::DataType::Float3, "aNormal"   },
        { Shader::DataType::Float2, "aTexCoord" }
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
            if (std::strcmp(m_texturesLoaded[j]->getPath().data(), str.C_Str()) != 0)
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