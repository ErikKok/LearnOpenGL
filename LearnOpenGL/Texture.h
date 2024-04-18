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
	Texture(uint32_t color);								// Constructor single color
	Texture(const std::vector<std::string>& faces);			// Constructor cubemap
	Texture(const Texture& other) = delete;					// Copy constructor
	Texture& operator=(const Texture& other) = delete;		// Copy assignment
	Texture(Texture&& other) noexcept = delete; 			// Move constructor	
	Texture& operator=(Texture&& other) noexcept = delete;	// Move assignment
	~Texture();												// Destructor

	void activeTexture() const;
	void bindTexture(unsigned int);
	void unbindTexture();

	const unsigned int getId() const { return m_id; };
	const std::string_view getType() const { return m_type; };
	const std::string_view getfileName() const { return m_fileName; };
	void setType(std::string type) { m_type = type; };
	void setfileName(std::string fileName) { m_fileName = fileName; };
	const int getBound() const { return m_boundTextureUnit; };
	void setBound(int textureUnit) { m_boundTextureUnit = textureUnit; };

private:
	unsigned int m_id{};
	int m_boundTextureUnit{ -1 };	// -1 == not bound to a texture unit
	uint32_t m_singleColor{0};		// 0  == not a single color texture (type will be singleColor)
	std::string m_type{ "undefined"};
	std::string m_filePath{};
	std::string m_fileName{};
	GLsizei m_width{};
	GLsizei m_height{};
};