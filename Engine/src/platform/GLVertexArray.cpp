#include <platform/GLVertexArray.h>

GLVertexArray::GLVertexArray()
{
    glCreateVertexArrays(1, &m_id);
}

GLVertexArray::~GLVertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void GLVertexArray::bind() const
{
    glBindVertexArray(m_id);
}

void GLVertexArray::unbind() const
{
    glBindVertexArray(0);
}

void GLVertexArray::addVertexBuffer(Shared<VertexBuffer> buffer)
{
    bind();
    buffer->bind();
    int i = 0;
    for (m_attribCount ; m_attribCount < buffer->getLayout().size() ; m_attribCount++)
    {
        GLenum type;
        switch (buffer->getLayout()[i].type)
        {
            using Type = Shader::DataType;

            case Type::Float:
            case Type::Vec2:
            case Type::Vec3:
            case Type::Vec4:
            case Type::Mat3:
            case Type::Mat4:
            case Type::Color:   type = GL_FLOAT; break;

            case Type::Bool:
            case Type::bVec2:
            case Type::bVec3:
            case Type::bVec4:
            case Type::Int:
            case Type::iVec2:
            case Type::iVec3:
            case Type::iVec4:   type = GL_INT; break;

            case Type::Uint:
            case Type::uVec2:
            case Type::uVec3:
            case Type::uVec4:   type = GL_UNSIGNED_INT; break;

            case Type::Double:
            case Type::dVec2:
            case Type::dVec3:
            case Type::dVec4:
            case Type::dMat3:
            case Type::dMat4:   type = GL_DOUBLE; break;

            default:            std::cout << "Unknown GLSL data type.\n";
        }

        glVertexAttribPointer(m_attribCount,
                              buffer->getLayout()[i].componentCount(),
                              type,
                              GL_FALSE,
                              buffer->getLayout().getStride(),
                              (const void*)buffer->getLayout()[i].offset);

        glEnableVertexAttribArray(m_attribCount);
        i++;
    }
}

void GLVertexArray::setIndexBuffer(Shared<IndexBuffer> buffer)
{
    bind();

    buffer->bind();

    m_pIndexBuffer = buffer;
}

Shared<IndexBuffer> GLVertexArray::getIndexBuffer() const
{
    return m_pIndexBuffer;
}