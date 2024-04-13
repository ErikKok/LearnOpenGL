#pragma once
#include "BufferSubData.h"

class UniformBuffer {
public:
	UniformBuffer(size_t size, GLuint index);
	UniformBuffer(const UniformBuffer& other) = delete;					// Copy constructor
	UniformBuffer& operator=(const UniformBuffer& other) = delete;		// Copy assignment
	UniformBuffer(UniformBuffer&& other) noexcept = delete; 			// Move constructor	
	UniformBuffer& operator=(UniformBuffer&& other) noexcept = delete;	// Move assignment
	~UniformBuffer();

	void bindUniformBuffer() const;
	void unbindUniformBuffer() const;

	void addUniformBufferSubData(BufferSubDataLayout& layout);

	const GLuint getId() const { return m_id; };

private:
	GLuint m_id{};
};