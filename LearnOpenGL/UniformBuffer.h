#pragma once

#include <glad/glad.h>

class UniformBuffer {
public:
	UniformBuffer(GLuint bindingPoint);
	//UniformBuffer(GLuint size, GLuint index); // converted to DSA & immutable + more on 28-5-2024 - UNTESTED SINCE!
	UniformBuffer(const UniformBuffer& other) = delete;					// Copy constructor
	UniformBuffer& operator=(const UniformBuffer& other) = delete;		// Copy assignment
	UniformBuffer(UniformBuffer&& other) noexcept = delete; 			// Move constructor	
	UniformBuffer& operator=(UniformBuffer&& other) noexcept = delete;	// Move assignment
	~UniformBuffer();

	GLuint getId() const { return m_id; };

	void bindUniformBuffer() const;
	void unbindUniformBuffer() const;

	//void addUniformBufferSubData(const BufferSubDataLayout& layout) const;

private:
	GLuint m_id{};
	GLuint m_bindingPoint{};
};