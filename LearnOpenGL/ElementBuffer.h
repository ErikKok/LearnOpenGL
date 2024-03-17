#pragma once

#include <glad/glad.h>

class ElementBuffer {
public:
    ElementBuffer(const GLvoid* data, GLuint size);
    ~ElementBuffer();

    void bindElementBuffer() const;
    void unbindElementBuffer() const;

    //inline unsigned int getCount() const;

private:
    GLuint m_id{};
    unsigned int m_count{};
};
