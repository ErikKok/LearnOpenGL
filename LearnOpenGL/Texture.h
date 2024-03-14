#pragma once
#include "Texture.h"

#include <string>

class Texture {
public:
	Texture(const std::string& filePath, unsigned int RGB_A = 0); // 1 sets RGB_A instead of RGB
	~Texture();

	void bindTexture(unsigned int slot = 0) const;
	void unbindTexture() const;

private:
	unsigned int m_Id{};
};