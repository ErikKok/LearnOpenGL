#pragma once

// First make a vector (e.g. ssbo1ModelViewVector):
// Define some max count int and resize or fill the vector

// Only then insert the data into the buffer with glNamedBufferStorage (or specify the size manually)
// The size of the the buffer is fixed for the rest of it's life span after executing glNamedBufferStorage
// glNamedBufferStorage is executed first with the maximum size you need, just use this once I guess.
//
// If you want to replace existing ssbo data, then use glNamedBufferSubData starting from byte 0
// glNamedBufferSubData(ssboModelView, 0, sizeof(glm::mat4)* ssbo1ModelViewVector.size(), (const void*)ssbo1ModelViewVector.data());
// 
// When you need to draw less then the maximum amount of elements of the vector
// You could either clear the vectors first...
//ssbo1ModelViewVector.clear();
//ssbo2NormalViewCPUVector.clear();
// ... and fill them again from element 0
//ssbo1ModelViewVector.emplace_back(modelView);
//ssbo2NormalViewCPUVector.emplace_back(glm::transpose(glm::inverse(modelView)));
// OR
// just fill element 0 if you just need to draw 1 instance
//ssbo1ModelViewVector[0] = modelView;
// Be sure not the use the other, not-updated, elements