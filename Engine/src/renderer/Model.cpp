#include <renderer/Model.h>
#include <renderer/Renderer3D.h>

Shared<Model> Model::loadModel(const std::string& file)
{
    auto model = createShared<Model>();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << "\n";
        return model;
    }

    model->m_directory = file.substr(0, file.find_last_of('/'));

    model->processNode(scene->mRootNode, scene);

    return model;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<ModelVertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Shared<Texture2D>> textures;

    for (int i = 0; i < mesh->mNumVertices; i++)
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

    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Shared<Texture2D>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    }

    Mesh mesh_;
    mesh_.vertexArray = VertexArray::create();
    mesh_.vertexArray->bind();

    BufferLayout layout = {
        { Shader::DataType::Vec3, "aPos"      },
        { Shader::DataType::Vec3, "aNormal"   },
        { Shader::DataType::Vec2, "aTexCoord" }
    };

    mesh_.indexBuffer = IndexBuffer::create(&indices[0], indices.size());
    
    mesh_.vertexBuffer = VertexBuffer::create(vertices.size() * sizeof(ModelVertex));
    mesh_.vertexBuffer->update(&vertices[0], vertices.size() * sizeof(ModelVertex));
    mesh_.vertexBuffer->setLayout(layout);
    
    mesh_.vertexArray->addVertexBuffer(mesh_.vertexBuffer);
    mesh_.vertexArray->setIndexBuffer(mesh_.indexBuffer);

    Shared<Material> material = Material::create(Renderer3D::data.modelShader);
    for (auto& texture : textures)
    {
        material->setTexture(texture);
    }

    mesh_.material = material;
    
    return mesh_;
}

std::vector<Shared<Texture2D>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
    std::vector<Shared<Texture2D>> textures;
    for (int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Shared<Texture2D> texture;
        //texture = Texture2D::create(m_directory + "/" + str.C_Str());
        texture = Texture2D::create("Sandbox/assets/grass.png");
        textures.push_back(texture);
    }

    return textures;
}