#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexAttribute.h"

#include <glad/glad.h>

#include <cassert>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_Id);
	glBindVertexArray(m_Id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_Id);
}

void VertexArray::bindVertexArray() const
{
	glBindVertexArray(m_Id);
}

void VertexArray::unbindVertexArray() const
{
	glBindVertexArray(0);
}

void VertexArray::addVertexAttributeLayout(const VertexBuffer& vb, VertexAttributeLayout& layout)
{
	vb.bindVertexBuffer();
	auto& vertexAttributes{ layout.getVertexAttributes() };
	vertexAttributes[0].offset = 0;
	for (GLuint i{ 0 }; i < vertexAttributes.size(); i++) {
		const auto& vertexAttribute{ vertexAttributes[i] };
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, vertexAttribute.size, vertexAttribute.type, vertexAttribute.normalized, layout.getStride(), (const void*)vertexAttribute.offset);
	}
}


//void VertexArray::addVertexAttributeLayout(const VertexBuffer& vb, VertexAttributeLayout& layout)
//{
//	vb.bindVertexBuffer();
//	const auto& vertexAttributes{ layout.getVertexAttributes() };
//	uintptr_t offset{ 0 };
//	for (GLuint i{ 0 }; i < vertexAttributes.size(); i++) {
//		const auto& vertexAttribute{ vertexAttributes[i] };
//		glEnableVertexAttribArray(i);
//		glVertexAttribPointer(i, vertexAttribute.size, vertexAttribute.type, vertexAttribute.normalized, layout.getStride(), (const void*)offset);
//		offset += vertexAttribute.offset;
//	}
//}