#pragma once

#include <glad/glad.h>

class VertexBuffer {
public:
    VertexBuffer(size_t size, const GLvoid* data);
    VertexBuffer(const VertexBuffer& other) = delete;					// Copy constructor
    VertexBuffer& operator=(const VertexBuffer& other) = delete;		// Copy assignment
    VertexBuffer(VertexBuffer&& other) noexcept = delete; 			    // Move constructor	
    VertexBuffer& operator=(VertexBuffer&& other) noexcept = delete;	// Move assignment
    ~VertexBuffer();

    GLuint getId() const { return m_id; };

    //void bindVertexBuffer() const;
    //void unbindVertexBuffer() const;

private:
    GLuint m_id{};
};

// size is in bytes of the buffer object's new data store.
// Usage:
// 
// With a 'single' data array:
// VertexBuffer name(sizeof(std::array), &std::array);
// 
// With a data array with multiple elements:
// VertexBuffer name(std::array.size() * sizeof(Type), &std::array[0])}
