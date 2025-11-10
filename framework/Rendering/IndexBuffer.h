#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <glad/glad.h>

class IndexBuffer
{
public:
    // Constructor. Initializes the class with a data buffer and its size.
    // Note: The buffer will be bound upon construction, and the size is
    // specified in the number of elements, not bytes.
    IndexBuffer(const GLuint *indices, GLsizei count);
    ~IndexBuffer();

    // Bind the vertex buffer.
    void Bind() const;

    // Unbind the vertex buffer.
    void Unbind() const;

    // Get the number of elements.
    inline GLuint GetCount() const { return Count; }

private:
    GLuint IndexBufferID;
    GLuint Count;
};

#endif // INDEXBUFFER_H