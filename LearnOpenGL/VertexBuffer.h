#pragma once

#include <glad/glad.h>

class VertexBuffer {
public:

    //VertexBuffer() = default; // TODO
    VertexBuffer(size_t size, const GLvoid* data);
    ~VertexBuffer();

    void bindVertexBuffer() const;
    void unbindVertexBuffer() const;

    const GLuint getId() const { return m_Id; };

private:
    GLuint m_Id{};
};

// size is in bytes of the buffer object's new data store.
// Usage:
// 
// With a 'single' data array:
// VertexBuffer name(sizeof(std::array), &std::array);
// 
// With a data array with multiple elements:
// VertexBuffer name(std::array.size() * sizeof(Type), &std::array[0])}
//
// //
// 
// .size() returns the number of elements in the vector.
//
// sizeof gives you the number of bytes used by the object definition(based on the declaration of the vector class)
//
// .size() * sizeof(type) = total size in bytes