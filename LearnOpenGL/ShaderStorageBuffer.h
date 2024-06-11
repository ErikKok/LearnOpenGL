#pragma once

#include "BufferDataStore.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

//There can only be one array of variable size per SSBO and it has to be the bottommost variable in the block definition.

enum ssboTypes { // is dit wel nodig?
	dirLightMVPMatrixSSBO = 0,
	flashLightMVPMatrixSSBO = 1,
	spotLightMVPMatrixSSBO = 2,
	normalMatrixSSBO = 3,
	modelViewMatrixSSBO = 4,
	MVPMatrixSSBO = 5,
};

enum ssboBindingPoints {
	normalMatrixBP = 2,
	modelViewMatrixBP = 3,
	MVPMatrixBP = 4,
	dirLightMVPMatrixBP = 5,
	spotLightMVPMatrixBP = 6,
	flashLightMVPMatrixBP = 7,
	singleColorBP = 20,
};

class ShaderStorageBuffer {
public:
	ShaderStorageBuffer(int bindingPoint, int elementCount, GLsizeiptr elementSize);
	ShaderStorageBuffer(const ShaderStorageBuffer& other) = delete;					// Copy constructor
	ShaderStorageBuffer& operator=(const ShaderStorageBuffer& other) = delete;		// Copy assignment
	ShaderStorageBuffer(ShaderStorageBuffer&& other) noexcept = delete; 			// Move constructor	
	ShaderStorageBuffer& operator=(ShaderStorageBuffer&& other) noexcept = delete;	// Move assignment
	~ShaderStorageBuffer();															// Destructor

	const GLuint getId() const { return m_id; };
	const int getBindingPoint() const { return m_bindingPoint; };

	void bind() const;
	//void unbind() const;

	// Pass-through functions BufferDataStore
	void addBufferSubData(const auto& data) { m_BufferDataStore.addBufferSubData(data); };
	//void createAndInitializeImmutableDataStore() { m_dataStore.createAndInitializeImmutableDataStore(); };
	void updateSubset(const auto& data, GLintptr elementIndex = 0) { m_BufferDataStore.updateSubset(data, elementIndex); };
	void updateAndUploadSubset(const auto& data, GLintptr elementIndex = 0) { m_BufferDataStore.updateAndUploadSubset(data, elementIndex); };
	void uploadFully()  { m_BufferDataStore.uploadFully(); }; // niet meer const vanwege m_hasBeenUploaded

private:
	GLuint m_id{};
	int m_bindingPoint{};
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