#pragma once

// First make a vector (e.g. ssbo1ModelViewVector):
// - fill it with the maximum amount of elements
// OR
// - reserve the maximum amount of elements
// Only then insert the data into the buffer with glNamedBufferStorage
// The size of the the buffer is fixed for the rest of it's life span after executing glNamedBufferStorage
// I guess you could manually specify a size @ glNamedBufferStorage, insert that into the buffer and fill the vector later. But you have to make sure to fill ALL the data before binding/using the ssbo. Untested.
//
// When you need to draw less then the maximum amount of elements of the vector
// You could either clear the vectors...
//ssbo1ModelViewVector.clear();
//ssbo2NormalViewCPUVector.clear();
// ... and fill them again from element 0
//ssbo1ModelViewVector.emplace_back(modelView);
//ssbo2NormalViewCPUVector.emplace_back(glm::transpose(glm::inverse(modelView)));
// OR
// just fill element 0 if you just need to draw 1 instance
//ssbo1ModelViewVector[0] = modelView;
// Be sure not the use the other, not-updated, elements

// Replace existing ssbo data starting from byte 0
//glNamedBufferSubData(ssboModelView, 0, sizeof(glm::mat4)* ssbo1ModelViewVector.size(), (const void*)ssbo1ModelViewVector.data());

