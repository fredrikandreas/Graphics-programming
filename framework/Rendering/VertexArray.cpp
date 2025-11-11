#include "VertexArray.h"
#include "ShadersDataTypes.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_vertexArrayID);
    Bind();
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
    const auto &attributes = layout.GetAttributes();
    int i = 0;
    for (const auto &attribute : attributes)
    {

        if (attribute.Name == "position")
        {
            i = 0;
        }
        else if (attribute.Name == "color")
        {
            i = 1;
        }
        else if (attribute.Name == "tcoords")
        {
            i = 2;
        }
        else if (attribute.Name == "normal")
        {
            i = 3;
        }

        glVertexAttribPointer(
            i,
            ShaderDataTypeComponentCount(attribute.Type),
            ShaderDataTypeToOpenGLBaseType(attribute.Type),
            attribute.Normalized,
            layout.GetStride(),
            reinterpret_cast<const void *>(attribute.Offset));

        glEnableVertexAttribArray(i);
    }

    VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
{
    Bind();
    indexBuffer->Bind();
    
    this->m_IdxBuffer = indexBuffer;
}