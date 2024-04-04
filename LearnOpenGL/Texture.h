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

	unsigned int m_Id{};
	std::string m_type{};
	std::string m_path;
private:

};