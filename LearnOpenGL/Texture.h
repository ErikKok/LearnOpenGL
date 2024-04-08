#pragma once
#include "Texture.h"

#include <print>
#include <string>

class Texture {
public:
	Texture(const std::string& filePath);				// constructor
	~Texture();											// destructor

	// TODO o.a. naar .cpp verplaatsen nog als het goed werkt.
	Texture(const Texture& other) = delete;				// copy constructor
	Texture& operator=(const Texture& other) = delete;	// copy assignment

	Texture(Texture&& other) noexcept 					// move constructor
	{
		m_id = other.m_id;
		m_type = other.m_type;
		m_filePath = other.m_filePath;
		m_fileName = other.m_fileName;
		m_width = other.m_width;
		m_height = other.m_height;

		other.m_id = 0u;
		other.m_type = "moved"; // TODO
		other.m_filePath = "";
		other.m_fileName = "";
		other.m_width = 0u;
		other.m_height = 0u;

		std::println("********************MOVE CONSTRUCTOR texture id: {}", m_id);
	}

	Texture& operator=(Texture&& other) noexcept		// Move assignment
	{		
		if (&other == this)
			return *this;

		m_id = other.m_id;
		m_type = other.m_type;
		m_filePath = other.m_filePath;
		m_fileName = other.m_fileName;
		m_width = other.m_width;
		m_height = other.m_height;

		other.m_id = 0u;
		other.m_type = "moved";
		other.m_filePath = "";
		other.m_fileName = "";
		other.m_width = 0u;
		other.m_height = 0u;

		std::println("********************MOVE ASSIGNMENT texture id: {}", m_id);

		return *this;
	}

	//void assignTextureUnit(TODO?);
	void bindTexture(unsigned int textureUnit = 0) const;
	void unbindTexture() const;

	unsigned int m_id{};
	std::string m_type{};
	std::string m_filePath{};
	std::string m_fileName{};
	GLsizei m_width{};
	GLsizei m_height{};
private:

};