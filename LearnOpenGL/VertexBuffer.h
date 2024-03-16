#pragma once

#include <glad/glad.h>

class VertexBuffer {
public:
    VertexBuffer(const GLvoid* data, GLuint size);
    ~VertexBuffer();

    void bindVertexBuffer() const;
    void unbindVertexBuffer() const;

private:
    GLuint m_Id{};
};