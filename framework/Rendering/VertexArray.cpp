#include "VertexArray.h"
#include "ShadersDataTypes.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_vertexArrayID);
}

VertexArray::~VertexArray()
{
    Unbind();
    glDeleteVertexArrays(1, &m_vertexArrayID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_vertexArrayID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer)
{
    Bind();
    vertexBuffer->Bind();

    const auto &layout = vertexBuffer->GetLayout();
    for (GLuint i = 0; i < layout.GetAttributes().size(); ++i)
    {
        const auto &attribute = layout.GetAttributes()[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i,
                              ShaderDataTypeComponentCount(attribute.Type),
                              ShaderDataTypeToOpenGLBaseType(attribute.Type),
                              attribute.Normalized,
                              layout.GetStride(),
                              (const void *)attribute.Offset);
    }

    VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
{
    Bind();
    indexBuffer->Bind();
    IdxBuffer = indexBuffer;
}