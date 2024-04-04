#pragma once

#include <glad/glad.h>

class ElementBuffer {
public:
    ElementBuffer(const GLvoid* data, GLuint size);
    ~ElementBuffer();

    void bindElementBuffer() const;
    void unbindElementBuffer() const;

    const GLuint getId() const { return m_Id; };
    //inline unsigned int getCount() const;

private:
    GLuint m_Id{};
    unsigned int m_Count{};
};
