#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void *vertices, GLsizei size)
{
    glGenBuffers(1, &VertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    Unbind();
    glDeleteBuffers(1, &VertexBufferID);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, const void *data) const
{
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
