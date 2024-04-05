#pragma once

#include <glad/glad.h>

class ElementBuffer {
public:
    ElementBuffer(size_t size, const GLvoid* data);
    ~ElementBuffer();

    void bindElementBuffer() const;
    void unbindElementBuffer() const;

    const GLuint getId() const { return m_id; };
    //inline unsigned int getCount() const;

private:
    GLuint m_id{};
};
