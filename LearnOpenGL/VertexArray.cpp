#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"

#include <glad/glad.h>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_id);
	glBindVertexArray(m_id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bindVertexArray() const
{
	glBindVertexArray(m_id);
}

void VertexArray::unbindVertexArray() const
{
	glBindVertexArray(0);
}

void VertexArray::addVertexAttributeLayout(const VertexBuffer& vb, VertexAttributeLayout& layout)
{
	vb.bindVertexBuffer();
	const auto& vertexAttributes{ layout.getVertexAttributes() };
	uintptr_t totalOffset{0};
	for (GLuint i{ 0 }; i < vertexAttributes.size(); i++) {
		const auto& vertexAttribute{ vertexAttributes[i] };
		glVertexAttribPointer(i, vertexAttribute.m_count, vertexAttribute.m_type, vertexAttribute.m_normalized, layout.getStride(), (const void*)totalOffset);
		glEnableVertexAttribArray(i);
		totalOffset += vertexAttribute.m_offset;
	}
}