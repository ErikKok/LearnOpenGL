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

    const GLuint getId() const { return m_id; };

private:
    GLuint m_id{};
};

