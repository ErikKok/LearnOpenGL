#pragma once
#include "Texture.h"

#include <array>
#include <optional>
#include <print>
#include <string>
#include <vector>

class Texture {
public:
	Texture(const std::string& filePath);					// Constructor
	Texture(const std::vector<std::string>& faces);			// Constructor cubemap
	Texture(const Texture& other) = delete;					// Copy constructor
	Texture& operator=(const Texture& other) = delete;		// Copy assignment
	Texture(Texture&& other) noexcept = delete; 			// Move constructor	
	Texture& operator=(Texture&& other) noexcept = delete;	// Move assignment
	~Texture();												// Destructor

	void activeTexture() const;
	void bindTexture(unsigned int textureUnit = 0) const;
	void unbindTexture();

	const unsigned int getId() const { return m_id; };
	const std::string_view getType() const { return m_type; };
	const std::string_view getfileName() const { return m_fileName; };
	void setType(std::string type) { m_type = type; };
	void setfileName(std::string fileName) { m_fileName = fileName; };
	const int getBound() const { return m_BoundTextureUnit; };
	void setBound(int textureUnit) { m_BoundTextureUnit = textureUnit; };

private:
	unsigned int m_id{};
	int m_BoundTextureUnit{ -1 }; // -1 == not bound
	std::string m_type{};
	std::string m_filePath{};
	std::string m_fileName{};
	GLsizei m_width{};
	GLsizei m_height{};
};