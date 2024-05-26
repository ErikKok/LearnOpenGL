#pragma once

#include "Buffers.h"
#include "VertexAttribute.h"

class VertexArray {
public:
    VertexArray();
    VertexArray(const VertexArray& other) = delete;					// Copy constructor
    VertexArray& operator=(const VertexArray& other) = delete;		// Copy assignment
    VertexArray(VertexArray&& other) noexcept = delete; 			// Move constructor	
    VertexArray& operator=(VertexArray&& other) noexcept = delete;	// Move assignment
    ~VertexArray();

    void bindVertexArray() const;
    void unbindVertexArray() const;

    void addVertexAttributeLayout(const VertexBuffer& vb, const VertexAttributeLayout& layout);

    const GLuint getId() const { return m_id; };

private:
    GLuint m_id{};
};

