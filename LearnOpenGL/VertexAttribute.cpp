//#pragma once
//#include "VertexAttribute.h"
//
//#include <glad/glad.h>
//
//VertexAttributeLayout::VertexAttributeLayout() {};
//
//template<typename T>
//void VertexAttributeLayout::pushVertexAttributeLayout(unsigned int count)
//{
//	static_assert(false);
//}
//
//template<>
//void VertexAttributeLayout::pushVertexAttributeLayout<float>(unsigned int count)
//{
//	m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
//	m_Stride += count * VertexAttribute::getSizeOfType(GL_FLOAT);
//}
//
//template<>
//void VertexAttributeLayout::pushVertexAttributeLayout<unsigned int>(unsigned int count)
//{
//	m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
//	m_Stride += count * VertexAttribute::getSizeOfType(GL_UNSIGNED_INT);
//}
//
//template<>
//void VertexAttributeLayout::pushVertexAttributeLayout<unsigned char>(unsigned int count)
//{
//	m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
//	m_Stride += count * VertexAttribute::getSizeOfType(GL_UNSIGNED_BYTE);
//}
//
//inline const std::vector<VertexAttribute>& VertexAttributeLayout::getElements() const
//{
//	return m_Elements;
//}
//
//inline unsigned int VertexAttributeLayout::getStride() const
//{
//	return m_Stride;
//}