#include <platform/GL/GLVertexArray.h>
#include <core/Logger.h>

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

void GLVertexArray::addVertexBuffer(const Shared<VertexBuffer>& buffer)
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
            case Type::Float2:
            case Type::Float3:
            case Type::Float4:
            case Type::Mat2:
            case Type::Mat2x2:
            case Type::Mat2x3:
            case Type::Mat2x4:
            case Type::Mat3x2:
            case Type::Mat3:
            case Type::Mat3x3:
            case Type::Mat3x4:
            case Type::Mat4x2:
            case Type::Mat4x3:
            case Type::Mat4:
            case Type::Mat4x4:  type = GL_FLOAT; break;

            case Type::Bool:
            case Type::Bool2:
            case Type::Bool3:
            case Type::Bool4:
            case Type::Int:
            case Type::Int2:
            case Type::Int3:
            case Type::Int4:    type = GL_INT; break;

            case Type::UInt:
            case Type::UInt2:
            case Type::UInt3:
            case Type::UInt4: type = GL_UNSIGNED_INT; break;

            case Type::Double:
            case Type::Double2:
            case Type::Double3:
            case Type::Double4:
            case Type::DMat2:
            case Type::DMat2x2:
            case Type::DMat2x3:
            case Type::DMat2x4:
            case Type::DMat3x2:
            case Type::DMat3:
            case Type::DMat3x3:
            case Type::DMat3x4:
            case Type::DMat4x2:
            case Type::DMat4x3:
            case Type::DMat4:
            case Type::DMat4x4:  type = GL_DOUBLE; break;

            default:            Logger::getCoreLogger()->error("Unknown GLSL data type."); type = 0; break;
        }

        bool normalized = buffer->getLayout()[i].normalized ? GL_TRUE : GL_FALSE;

        glVertexAttribPointer(m_attribCount,
                              buffer->getLayout()[i].componentCount(),
                              type,
                              normalized,
                              buffer->getLayout().getStride(),
                              (const void*)buffer->getLayout()[i].offset);

        glEnableVertexAttribArray(m_attribCount);
        i++;
    }
}

void GLVertexArray::setIndexBuffer(const Shared<IndexBuffer>& buffer)
{
    bind();

    buffer->bind();

    m_indexBuffer = buffer;
}