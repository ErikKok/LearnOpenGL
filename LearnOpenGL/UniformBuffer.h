#pragma once
#include "Global.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cassert>
#include <print>

class UniformBuffer {
public:
	UniformBuffer(int bindingPoint);
	//UniformBuffer(GLuint size, GLuint index); // converted to DSA & immutable + more on 28-5-2024 - UNTESTED SINCE!
	UniformBuffer(const UniformBuffer& other) = delete;					// Copy constructor
	UniformBuffer& operator=(const UniformBuffer& other) = delete;		// Copy assignment
	UniformBuffer(UniformBuffer&& other) noexcept = delete; 			// Move constructor	
	UniformBuffer& operator=(UniformBuffer&& other) noexcept = delete;	// Move assignment
	~UniformBuffer();

	const GLuint getId() const { return m_id; };

	void bindUniformBuffer() const;
	void unbindUniformBuffer() const;

	//void addUniformBufferSubData(const BufferSubDataLayout& layout) const;

private:
	GLuint m_id{};
	int m_bindingPoint{};
};