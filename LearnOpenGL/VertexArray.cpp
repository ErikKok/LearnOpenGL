#pragma once
#include "VertexArray.h"

#include "Global.h"
#include "VertexBuffer.h"

#include <glad/glad.h>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_Id);
	glBindVertexArray(m_Id);
	Global::glCheckError();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_Id);
	Global::glCheckError();
}

void VertexArray::bindVertexArray() const
{
	glBindVertexArray(m_Id);
	Global::glCheckError();
}

void VertexArray::unbindVertexArray() const
{
	glBindVertexArray(0);
	Global::glCheckError();
}

void VertexArray::addVertexAttributeLayout(const VertexBuffer& vb, VertexAttributeLayout& layout)
{
	vb.bindVertexBuffer();
	const auto& vertexAttributes{ layout.getVertexAttributes() };
	uintptr_t totalOffset{0};
	for (GLuint i{ 0 }; i < vertexAttributes.size(); i++) {
		const auto& vertexAttribute{ vertexAttributes[i] };
		glVertexAttribPointer(i, vertexAttribute.m_Count, vertexAttribute.m_Type, vertexAttribute.m_Normalized, layout.getStride(), (const void*)totalOffset);
		glEnableVertexAttribArray(i);
		totalOffset += vertexAttribute.m_Offset;
	}
	Global::glCheckError();
}