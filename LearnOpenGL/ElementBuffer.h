#pragma once

#include <glad/glad.h>

// AKA IndexBuffer

class ElementBuffer {
public:
    ElementBuffer(size_t size, const GLvoid* data);
    ElementBuffer(const ElementBuffer& other) = delete;					// Copy constructor
    ElementBuffer& operator=(const ElementBuffer& other) = delete;		// Copy assignment
    ElementBuffer(ElementBuffer&& other) noexcept = delete; 			// Move constructor	
    ElementBuffer& operator=(ElementBuffer&& other) noexcept = delete;	// Move assignment
    ~ElementBuffer();

    GLuint getId() const { return m_id; };
    GLuint getCount() const { return m_count; };

    //void bindElementBuffer() const;
    //void unbindElementBuffer() const;

private:
    GLuint m_id{};
    GLuint m_count{};
};
