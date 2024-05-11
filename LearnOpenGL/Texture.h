#pragma once
#include "Texture.h"

#include <array>
#include <optional>
#include <print>
#include <string>
#include <vector>

enum textureType {
	undefined,
	diffuse,
	specular,
	normal,
	height,
	singleColor,
	cubeMap,
	depthMap,
};

constexpr std::array textureTypeName{ "undefined", "diffuse", "specular", "normal", "height", "singleColor", "cubeMap", "depthMap" };

class Texture {
public:
	Texture(const std::string& filePath, bool convertToLinearSpace = true);	// Constructor	// Default converts to Linear Space
	Texture(uint32_t color);								// Constructor single color		// Always converts to Linear Space
	Texture(const std::vector<std::string>& faces);			// Constructor cubemap			// Always converts to Linear Space
	Texture(const Texture& other) = delete;					// Copy constructor
	Texture& operator=(const Texture& other) = delete;		// Copy assignment
	Texture(Texture&& other) noexcept = delete; 			// Move constructor	
	Texture& operator=(Texture&& other) noexcept = delete;	// Move assignment
	~Texture();												// Destructor

	void activeTexture() const;
	void bindTexture(unsigned int);
	void unbindTexture();

	const unsigned int getId() const { return m_id; };
	const textureType getType() const { return m_type; };
	const std::string getTypeAsString() const { return textureTypeName[m_type]; };
	void setType(textureType type) { m_type = type; };
	const std::string_view getfileName() const { return m_fileName; };
	void setfileName(std::string fileName) { m_fileName = fileName; };
	const int getBound() const { return m_boundTextureUnit; };
	void setBound(int textureUnit) { m_boundTextureUnit = textureUnit; };

private:
	unsigned int m_id{};
	int m_boundTextureUnit{ -1 };	// -1 == not bound to a texture unit
	uint32_t m_singleColor{0};		// 0  == not a single color texture (type will be singleColor)
	textureType m_type{ textureType::undefined};
	std::string m_filePath{};
	std::string m_fileName{};
	GLsizei m_width{};
	GLsizei m_height{};
};