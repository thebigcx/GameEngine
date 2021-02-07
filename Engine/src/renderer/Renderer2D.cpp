#include <renderer/Renderer2D.h>
#include <core/Game.h>
#include <renderer/MeshFactory.h>
#include <maths/matrix/matrix_transform.h>
#include <maths/matrix/matrix_func.h>
#include <renderer/RenderCommand.h>
#include <renderer/Assets.h>

namespace Engine
{

void Renderer2D::init()
{
    s_data.matrixData = UniformBuffer::create(sizeof(math::mat4) * 2, 2);

    s_data.textShader = Shader::createFromFile("Engine/assets/shaders/Engine2D_Text.glsl");
    s_data.textureShader = Shader::createFromFile("Engine/assets/shaders/Engine2D_Texture.glsl");

    s_data.textureShader = Assets::get<Shader>("Engine2D_Texture");
    s_data.textShader = Assets::get<Shader>("Engine2D_Text");

    s_data.textMesh = MeshFactory::textMesh(s_data.MAX_GLYPHS);
    s_data.textVertexBase = new GlyphVertex[4 * s_data.MAX_GLYPHS];
    s_data.textVertexPtr = s_data.textVertexBase;

    s_data.vertexBase = new QuadVertex[s_data.MAX_VERTICES];

    s_data.mesh.vertexArray = VertexArray::create();
    s_data.mesh.vertexArray->bind();

    BufferLayout layout = {
        { Shader::DataType::Float3,  "aPos"      },
        { Shader::DataType::Float2,  "aTexCoord" },
        { Shader::DataType::Float4,  "aColor"    },
        { Shader::DataType::Float,   "aTexIndex" }
    };
    
    uint32_t* indices = new uint32_t[s_data.MAX_INDICES];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_data.MAX_SPRITES * 6; i += 6)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;

        offset += 4;
    }

    s_data.mesh.indexBuffer = IndexBuffer::create(indices, s_data.MAX_SPRITES * 6, IndexDataType::UInt32);

    delete[] indices;
 
    s_data.mesh.vertexBuffer = VertexBuffer::create(sizeof(QuadVertex) * 4 * s_data.MAX_SPRITES);
    s_data.mesh.vertexBuffer->setLayout(layout);

    s_data.mesh.vertexArray->addVertexBuffer(s_data.mesh.vertexBuffer);
    s_data.mesh.vertexArray->setIndexBuffer(s_data.mesh.indexBuffer);

    int32_t samplers[s_data.MAX_TEXTURE_SLOTS];
    for (int32_t i = 0; i < static_cast<int32_t>(s_data.MAX_TEXTURE_SLOTS); i++)    
    {
        samplers[i] = i;
    }

    s_data.textureShader->bind();
    s_data.textureShader->setIntArray("uTextures", samplers, s_data.MAX_TEXTURE_SLOTS);

    s_data.textureSlots[0] = Texture2D::createWhiteTexture();
}

void Renderer2D::shutdown()
{
    delete[] s_data.vertexBase;
    delete[] s_data.textVertexBase;
}

void Renderer2D::beginScene(Camera& camera)
{
    s_data.matrixData->setData(math::buffer(camera.getProjectionMatrix()), sizeof(math::mat4), 0);
    s_data.matrixData->setData(math::buffer(camera.getViewMatrix()), sizeof(math::mat4), sizeof(math::mat4));

    startBatch();
}

void Renderer2D::beginScene(Camera& camera, const math::mat4& transform)
{
    s_data.matrixData->setData(math::buffer(camera.getProjectionMatrix()), sizeof(math::mat4), 0);
    s_data.matrixData->setData(math::buffer(math::inverse<float>(transform)), sizeof(math::mat4), sizeof(math::mat4));

    startBatch();
}

void Renderer2D::endScene()
{
    flushBatch();
}

void Renderer2D::startBatch()
{
    s_data.textureSlotIndex = 1;
    s_data.vertexPointer = s_data.vertexBase;
    s_data.indexCount = 0;

    s_data.textVertexPtr = s_data.textVertexBase;
}

void Renderer2D::nextBatch()
{
    flushBatch();
    startBatch();
}

void Renderer2D::flushBatch()
{
    if (s_data.indexCount == 0)
        return;

    RenderCommand::setDepthTesting(false);

    s_data.mesh.vertexArray->bind();

    size_t dataSize = static_cast<size_t>(reinterpret_cast<uint8_t*>(s_data.vertexPointer) - reinterpret_cast<uint8_t*>(s_data.vertexBase));
    s_data.mesh.vertexBuffer->setData(s_data.vertexBase, dataSize);

    s_data.textureShader->bind();
    
    for (uint32_t i = 0; i < s_data.textureSlotIndex; i++)
    {
        s_data.textureSlots[i]->bind(i);
    }

    RenderCommand::renderIndexed(s_data.mesh.vertexArray, s_data.indexCount);
}

void Renderer2D::renderSprite(const Reference<Texture2D>& texture, const math::vec2& position, const math::vec2& size, const math::vec4& color)
{
    renderSprite(texture, position, size, math::frect(0, 0, texture->getWidth(), texture->getHeight()), 0, color);
}

void Renderer2D::renderSprite(const Reference<Texture2D>& texture, const math::vec2& position, const math::vec2& size, const math::frect& texRect)
{
    renderSprite(texture, position, size, texRect, 0, math::vec2(), math::vec4(1, 1, 1, 1));
}

void Renderer2D::renderSprite(const Reference<Texture2D>& texture, const math::vec2& position, const math::vec2& size, const math::frect& texRect, float rotation, math::vec4 color)
{
    renderSprite(texture, position, size, texRect, rotation, math::vec2(), color);
}

void Renderer2D::renderSprite(const Reference<Texture2D>& texture, const math::mat4& transform, const math::frect& texRect, const math::vec4& color)
{
    constexpr size_t quadVertexCount = 4;

    const float x1 = texRect.x / texture->getWidth();
    const float y1 = texRect.y / texture->getHeight();
    const float x2 = (texRect.x + texRect.w) / texture->getWidth();
    const float y2 = (texRect.y + texRect.h) / texture->getHeight();

    const math::vec2 texCoords[] = {
        { x1, y1 },
        { x1, y2 },
        { x2, y2 },
        { x2, y1 }
    };

    float textureIndex = 0.f;
    for (uint32_t i = 0; i < s_data.textureSlotIndex; i++)
    {
        if (*(s_data.textureSlots[i]) == *texture)
        {
            textureIndex = static_cast<float>(i);
            break;
        }
    }

    if (textureIndex == 0.f)
    {
        if (s_data.textureSlotIndex >= Renderer2DData::MAX_TEXTURE_SLOTS)
        {
            nextBatch();
        }

        textureIndex = static_cast<float>(s_data.textureSlotIndex);
        s_data.textureSlots[s_data.textureSlotIndex] = texture;
        s_data.textureSlotIndex++;
    }

    if (s_data.indexCount >= s_data.MAX_INDICES)
    {
        nextBatch();
    }
    
    // Populate the vertices array with the sprite's vertices
    for (size_t i = 0 ; i < quadVertexCount ; i++)
    {
        s_data.vertexPointer->position = transform * math::vec4(s_data.quadPositions[i], 0, 1);
        s_data.vertexPointer->texCoord = texCoords[i];
        s_data.vertexPointer->color = color;
        s_data.vertexPointer->texIndex = textureIndex;
        s_data.vertexPointer++;
    }

    s_data.indexCount += 6;
}

void Renderer2D::renderSprite(const Reference<Texture2D>& texture, const math::mat4& transform)
{
    renderSprite(texture, transform, math::frect(0, 0, texture->getWidth(), texture->getHeight()), math::vec4(1, 1, 1, 1));
}

void Renderer2D::renderSprite(const Reference<Texture2D>& texture, const math::vec2& position, const math::vec2& size, const math::frect& texRect, float rotation, const math::vec2& origin, math::vec4 color)
{
    Transform transform = { math::vec3(position), math::vec3(0, 0, rotation), math::vec3(size) }; // TODO: make static method
    renderSprite(texture, transform.matrix(), texRect, color);
}

void Renderer2D::renderQuad(const math::vec2& position, const math::vec2& size, const math::vec4& color)
{
    renderQuad(position, size, 0, color);
}

void Renderer2D::renderQuad(const math::vec2& position, const math::vec2& size, float rotation, const math::vec4& color)
{
    renderQuad(position, size, rotation, color, math::vec2(0, 0));
}

void Renderer2D::renderQuad(const math::vec2& position, const math::vec2& size, float rotation, const math::vec4& color, const math::vec2& origin)
{
    Transform transform = { math::vec3(position), math::vec3(0, 0, rotation), math::vec3(size) };
    renderQuad(transform.matrix(), color);
}

void Renderer2D::renderQuad(const math::mat4& transform, const math::vec4& color)
{
    constexpr uint32_t quadVertexCount = 4;
    constexpr float textureIndex = 0.f;
    constexpr math::vec2 texCoords[] = { {0, 0}, {0, 1}, {1, 1}, {1, 0} };

    for (uint32_t i = 0 ; i < quadVertexCount ; i++)
    {
        s_data.vertexPointer->position = math::vec3(transform * math::vec4(s_data.quadPositions[i]));
        s_data.vertexPointer->texCoord = texCoords[i];
        s_data.vertexPointer->color = color;
        s_data.vertexPointer->texIndex = textureIndex;
        s_data.vertexPointer++;
    }

    s_data.indexCount += 6;
}

void Renderer2D::renderText(const std::string& text, const Reference<TrueTypeFont>& font, const math::vec2& position, const math::vec4& color)
{
    renderText(text, font, position, math::vec2(font->getCharacterSize()), color);
}

void Renderer2D::renderText(const std::string& text, const Reference<TrueTypeFont>& font, const math::vec2& position, const math::vec2& size, const math::vec4& color)
{
    math::vec2 scale = size / (float)font->getCharacterSize();

    int x = position.x;
    int y = position.y;

    // Loop through each character in the string, and add its texCoords to the mesh
    for (auto& c : text)
    {
        auto& ch = font->getGlyphs().at(c);

        math::vec2 pos = { x + ch.pos.x * scale.x, -y - ch.pos.y * scale.y };
        math::vec2 size = ch.size * scale;

        x += ch.advance.x * scale.x;
        y += ch.advance.y * scale.y;

        float x1 = pos.x;
        float y1 = -pos.y;
        float x2 = pos.x + size.x;
        float y2 = -pos.y - size.y;

        float u1 = ch.texOffset;
        float v1 = 0;
        float u2 = ch.texOffset + ch.size.x / font->getAtlasSize().x;
        float v2 = ch.size.y / font->getAtlasSize().y;

        if (!size.x || !size.y)
        {
            continue;
        }

        s_data.textVertexPtr->position = { x1, y1 };
        s_data.textVertexPtr->texCoord = { u1, v1 };
        s_data.textVertexPtr++;

        s_data.textVertexPtr->position = { x2, y1 };
        s_data.textVertexPtr->texCoord = { u2, v1 };
        s_data.textVertexPtr++;

        s_data.textVertexPtr->position = { x2, y2 };
        s_data.textVertexPtr->texCoord = { u2, v2 };
        s_data.textVertexPtr++;

        s_data.textVertexPtr->position = { x1, y2 };
        s_data.textVertexPtr->texCoord = { u1, v2 };
        s_data.textVertexPtr++;
    }

    size_t dataSize = static_cast<size_t>(reinterpret_cast<uint8_t*>(s_data.textVertexPtr) - reinterpret_cast<uint8_t*>(s_data.textVertexBase));

    s_data.textMesh->vertexArray->bind();
    s_data.textMesh->vertexBuffer->setData(s_data.textVertexBase, dataSize);

    s_data.textShader->bind();
    s_data.textShader->setFloat4("uTextColor", color);
    font->getTextureAtlas()->bind();

    RenderCommand::setBlend(true);
    RenderCommand::setBlendFunction(BlendFunction::SourceAlpha, BlendFunction::OneMinusSourceAlpha);
    RenderCommand::setDepthTesting(false);

    RenderCommand::renderIndexed(s_data.textMesh->vertexArray, 6 * text.size());

    RenderCommand::setBlend(false);

    s_data.textVertexPtr = s_data.textVertexBase;
}

}