#pragma once

class ElementBuffer {
public:
    ElementBuffer(const void* data, unsigned int count);
    ~ElementBuffer();

    void bindElementBuffer() const;
    void unbindElementBuffer() const;

    inline unsigned int getCount() const;

private:
    unsigned int m_Id{};
    unsigned int m_Count{};
};
