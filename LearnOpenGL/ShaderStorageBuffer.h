#pragma once

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

// TODO what if you want to change the buffer size?
// Do you need to delete ssbo and re-init?
// I guess...