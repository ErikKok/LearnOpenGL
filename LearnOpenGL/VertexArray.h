#pragma once

#include "Buffers.h"
#include "Global.h"

#include <glad/glad.h>

#include <cassert>
#include <print>
#include <vector>

class VertexAttributeLayout;

class VertexArray {
public:
    VertexArray();
    VertexArray(const VertexArray& other) = delete;					// Copy constructor
    VertexArray& operator=(const VertexArray& other) = delete;		// Copy assignment
    VertexArray(VertexArray&& other) noexcept = delete; 			// Move constructor	
    VertexArray& operator=(VertexArray&& other) noexcept = delete;	// Move assignment
    ~VertexArray();

    const GLuint getId() const { return m_id; };

    void bindVertexArray() const;
    //void unbindVertexArray() const;

    void finalizeVertexAttributeLayout(const VertexBuffer* vbo, const VertexAttributeLayout* layout) const;

private:
    GLuint m_id{};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class VertexAttribute
{
	friend class VertexArray;
	friend class VertexAttributeLayout;

private:
	VertexAttribute(GLenum type, GLint count, GLboolean normalized, GLuint offset);

	GLenum m_type{};
	GLint m_count{}; // number of components/types of this attribute (Caution! named size @ docs.gl)
	GLboolean m_normalized{};
	GLuint m_offset{}; // size in bytes of this attribute
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class VertexAttributeLayout {
	friend class VertexArray;

public:
	VertexAttributeLayout() {};

	template<typename T>
	void pushVertexAttributeLayout(GLint count);
	template<>
	void pushVertexAttributeLayout<float>(GLint count);
	template<>
	void pushVertexAttributeLayout<int>(GLint count);
	template<>
	void pushVertexAttributeLayout<unsigned int>(GLint count);
	template<>
	void pushVertexAttributeLayout<unsigned char>(GLint count);

	//inline const std::vector<VertexAttribute>& getVertexAttributes() const { return m_vertexAttributes; }

	inline void setVertexAttributeOffset(int vertexAttributeIndex, int offsetNew);

	//inline const unsigned int getStride() const { return m_stride; }
	// Sets a custom Stride (the same for all the vertex attributes in the array)
	//inline void setVertexStride(GLsizei strideNew) { m_stride = strideNew; }
	// Specifies a custom offset of the vertexAttribute
	// Usage example 1:	if there is x bytes unused between attribute y and attribute y+1
	//					use index y+1, offsetNew x
	// Usage example 2:	if attribute y+1 reuses the last x bytes of attribute y
	//					use index y+1, offsetNew -x

private:
	std::vector<VertexAttribute> m_vertexAttributes{};
	GLuint m_stride{};
};