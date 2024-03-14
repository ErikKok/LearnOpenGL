#pragma once

class VertexBuffer {
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void bindVertexBuffer() const;
    void unbindVertexBuffer() const;

private:
    unsigned int m_Id{};
};