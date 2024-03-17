#pragma once
#include "Texture.h"

#include <string>

class Texture {
public:
	Texture(const std::string& filePath);
	~Texture();

	void bindTexture(unsigned int slot = 0) const;
	void unbindTexture() const;

private:
	unsigned int m_id{};
};