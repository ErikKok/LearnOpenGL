#pragma once

#include <glad/glad.h>

class ElementBuffer {
public:
    ElementBuffer(size_t size, const GLvoid* data);
    ElementBuffer(const ElementBuffer& other) = delete;					// Copy constructor
    ElementBuffer& operator=(const ElementBuffer& other) = delete;		// Copy assignment
    ElementBuffer(ElementBuffer&& other) noexcept = delete; 			// Move constructor	
    ElementBuffer& operator=(ElementBuffer&& other) noexcept = delete;	// Move assignment
    ~ElementBuffer();

    void bindElementBuffer() const;
    void unbindElementBuffer() const;

    const GLuint getId() const { return m_id; };

private:
    GLuint m_id{};
};
