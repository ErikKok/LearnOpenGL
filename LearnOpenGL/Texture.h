#pragma once
#include "Texture.h"

#include <string>

class Texture {
public:
	Texture(const std::string& filePath);
	~Texture();

	//void assignTextureUnit(TODO?);
	void bindTexture(unsigned int textureUnit = 0) const;
	void unbindTexture() const;

private:
	unsigned int m_id{};
};