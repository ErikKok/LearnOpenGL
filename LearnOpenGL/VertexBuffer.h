#pragma once

#include <glad/glad.h>

class VertexBuffer {
public:

    //VertexBuffer() = default; // TODO
    VertexBuffer(const GLvoid* data, GLuint size);
    ~VertexBuffer();

    void bindVertexBuffer() const;
    void unbindVertexBuffer() const;

    const GLuint getId() const { return m_Id; };

private:
    GLuint m_Id{};
};