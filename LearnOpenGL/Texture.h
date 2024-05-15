#pragma once

#include <array>
#include <optional>
#include <print>
#include <string>
#include <vector>

enum class textureType {
	undefined,
	diffuse,
	specular,
	normal,
	height,
	singleColor,
	cubeMap,
	depthMap,
	max_textureTypes,
};

constexpr std::array textureTypeName{ "undefined", "diffuse", "specular", "normal", "height", "singleColor", "cubeMap", "depthMap" };
static_assert(std::size(textureTypeName) == static_cast<int>(textureType::max_textureTypes));

class Texture {
public:
	Texture(const std::string& filePath, bool convertToLinearSpace = true);	// Constructor	// Default converts to Linear Space
	Texture(uint32_t color);								// Constructor single color		// Always converts to Linear Space
	Texture(const std::vector<std::string>& faces);			// Constructor cubeMap			// Always converts to Linear Space
	Texture(textureType textureType, GLsizei width, GLsizei height); // Constructor depthMap
	Texture(const Texture& other) = delete;					// Copy constructor
	Texture& operator=(const Texture& other) = delete;		// Copy assignment
	Texture(Texture&& other) noexcept = delete; 			// Move constructor	
	Texture& operator=(Texture&& other) noexcept = delete;	// Move assignment
	~Texture();												// Destructor

	void activeTexture() const;
	void bindTexture(GLuint);
	void unbindTexture();

	const GLuint getId() const { return m_id; };
	const int getBound() const { return m_boundTextureUnit; };
	void setBound(int textureUnit) { m_boundTextureUnit = textureUnit; };
	const textureType getType() const { return m_type; };
	const std::string getTypeAsString() const { return textureTypeName[static_cast<int>(m_type)]; };
	void setType(textureType type) { m_type = type; };
	const std::string_view getfileName() const { return m_fileName; };
	void setfileName(std::string fileName) { m_fileName = fileName; };
	const GLuint getWidth() const { return m_width; };
	const GLuint getHeight() const { return m_height; };

private:
	GLuint m_id{};
	int m_boundTextureUnit{ -1 };	// -1 == not bound to a texture unit
	uint32_t m_singleColor{0};		// 0  == not a single color texture (type will be singleColor)
	textureType m_type{ textureType::undefined};
	std::string m_filePath{};
	std::string m_fileName{};
	GLsizei m_width{};
	GLsizei m_height{};
};

// TODO
// Shadowmap: choose GL_DEPTH_COMPONENT bit: GL_DEPTH_COMPONENT32 is probably overkill, and as of 4.5, isn't a required format for OpenGL implementations to support (though 16 and 24 bit ones are)