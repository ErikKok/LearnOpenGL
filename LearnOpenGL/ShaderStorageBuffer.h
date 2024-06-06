#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderStorageBuffer {
public:
	ShaderStorageBuffer(int bindingPoint);											// Constructor
	ShaderStorageBuffer(int bindingPoint, int arrayCount);							// Constructor
	ShaderStorageBuffer(const ShaderStorageBuffer& other) = delete;					// Copy constructor
	ShaderStorageBuffer& operator=(const ShaderStorageBuffer& other) = delete;		// Copy assignment
	ShaderStorageBuffer(ShaderStorageBuffer&& other) noexcept = delete; 			// Move constructor	
	ShaderStorageBuffer& operator=(ShaderStorageBuffer&& other) noexcept = delete;	// Move assignment
	~ShaderStorageBuffer();															// Destructor

	const GLuint getId() const { return m_id; }; // was unsigned int 5-6-24

	void bind() const;
	void unbind() const;
	void upload() const;
	void update(const glm::mat4& vector, int i = 0);
	void uploadAndBind() const;
	void updateAndUpload(const glm::mat4& vector, int i);
	void updateAndUploadAndBind(const glm::mat4& vector, int i = 0);

private:
	GLuint m_id{};
	int m_bindingPoint{};
	int m_arrayCount{};
	std::vector<glm::mat4> m_vector{}; // TODO other types
	//BufferDataStore m_dataStore; // TODO
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