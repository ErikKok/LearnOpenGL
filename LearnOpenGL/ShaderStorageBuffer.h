#pragma once

#include "BufferDataStore.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

//There can only be one array of variable size per SSBO and it has to be the bottommost variable in the block definition.

// This magic enum can be used as element-index, as type and/or as binding point
// If used as binding point, you must use it as type as well
enum class SSBO : GLuint {\
	dirLightMVP = 0,
	spotLight0MVP = 1,
	spotLight1MVP = 2,
	normalMatrix = 3,
	modelViewMatrix = 4,
	MVP = 5,

	genericDepthMap = 8,
	singleColor = 20,

	undefined = 99,
};

// Overload the unary + operator to convert SSBO to the underlying type
constexpr auto operator+(SSBO x) noexcept
{
	return std::to_underlying(x);
}

//enum ssboBindingPoints {
//	dirLightMVPMatrixBP = 0,
//	spotLight0MVPMatrixBP = 1,
//	spotLight1MVPMatrixBP = 2,
//	normalMatrixBP = 3,
//	modelViewMatrixBP = 4,
//	MVPMatrixBP = 5,
//
//	genericDepthMapBP = 8,
//	singleColorBP = 20,
//	undefinedSSBO = 99,
//};

class ShaderStorageBuffer {
public:
	ShaderStorageBuffer(GLuint bindingPoint, int elementCount, GLsizeiptr elementSize, SSBO type = SSBO::undefined);
	ShaderStorageBuffer(const ShaderStorageBuffer& other) = delete;					// Copy constructor
	ShaderStorageBuffer& operator=(const ShaderStorageBuffer& other) = delete;		// Copy assignment
	ShaderStorageBuffer(ShaderStorageBuffer&& other) noexcept = delete; 			// Move constructor	
	ShaderStorageBuffer& operator=(ShaderStorageBuffer&& other) noexcept = delete;	// Move assignment
	~ShaderStorageBuffer();															// Destructor

	const GLuint getId() const { return m_id; };
	const SSBO getType() const { return m_ssboType; };
	//void setType(SSBO type) { m_ssboType = type; };
	//const GLuint getBindingPoint() const { return m_bindingPoint; };

	void bind() const;
	void bindOverrideBindingPoint(GLuint BP) const;
	//void unbind() const;

	// Pass-through functions BufferDataStore
	void updateSubset(const auto& data, GLintptr elementIndex, bool upload) { m_BufferDataStore.updateSubset(data, elementIndex, upload); };
	void updateFully(const auto& data, bool upload) { m_BufferDataStore.updateFully(data, upload); };
	void uploadUntilSubset(int untilElementIndex = 1) { m_BufferDataStore.uploadUntilSubset(untilElementIndex); };
	void uploadFully() { m_BufferDataStore.uploadFully(); };

private:
	GLuint m_id{};
	SSBO m_ssboType{ SSBO::undefined };
	GLuint m_bindingPoint{};
	BufferDataStore m_BufferDataStore;
};

// USAGE (without this class)
// First define some max count int (e.g. ssboArrayCount) 
// Make and initialize an array (e.g. ssbo1ModelViewArray)
// Array can be left empty or filled at this point

// Insert the data into the buffer with glNamedBufferStorage
// The size of the the buffer is fixed for the rest of it's life span after executing glNamedBufferStorage
// So there is no need to use a vector
// glNamedBufferStorage is executed first with the maximum size you need, just use this only once I guess
//
// If you want to replace existing ssbo data, then use glNamedBufferSubData starting from byte 0, e.g.:
// glNamedBufferSubData(ssboModelView, 0, sizeof(glm::mat4)* ssbo1ModelViewArray.size(), (const void*)ssbo1ModelViewArray.data());
// 
// When you need to draw less then the maximum amount of elements of the array
// 
// Fill the array per element using a loop
// OR
// just fill element 0 if you just need to draw 1 instance
// ssbo1ModelViewArray[0] = modelView;
// Be sure not the use the other, not-updated, elements
// Use the same index for the draw call as used in the loop to guard against this
// 
// And be sure not to fill beyond  the array capacity
// you can use an assert:
// assert(std::size(Data::cubePositions) <= ssboArrayCount && "Loop will create more instances then ssbo can hold");
// to guard you against this

// https://computergraphics.stackexchange.com/questions/5323/dynamic-array-in-glsl