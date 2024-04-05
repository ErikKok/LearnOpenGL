#pragma once
#include "BufferSubData.h"

class UniformBuffer {
public:
	UniformBuffer(size_t size, GLuint index);
	~UniformBuffer();

	void bindUniformBuffer() const;
	void unbindUniformBuffer() const;

	void addUniformBufferSubData(const UniformBuffer& ub, BufferSubDataLayout& layout);

	const GLuint getId() const { return m_Id; };

private:
	GLuint m_Id{};
};