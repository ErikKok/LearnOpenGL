#pragma once
#include "VertexAttribute.h"
#include "VertexBuffer.h"

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void bindVertexArray() const;
    void unbindVertexArray() const;

    void addVertexAttributeLayout(const VertexBuffer& vb, VertexAttributeLayout& layout);

private:
    GLuint m_id{};
};
