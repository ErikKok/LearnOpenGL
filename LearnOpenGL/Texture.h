#pragma once
#include "Texture.h"

class Texture {
public:
	Texture(const char* path, unsigned int RGB_A = 0);
	~Texture();

	void bindTexture(unsigned int slot = 0) const;
	void unbindTexture() const;

private:
	unsigned int m_Id{};
};