#pragma once
#include "Texture.h"

#include <glad/glad.h>
#include <STB/stb_image.h>

#include <print>
#include <string>

Texture::Texture(const std::string& filePath, unsigned int RGB_A)
{
    int textureWidth{};
    int textureHeight{};
    int textureNrChannels{};

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* textureData{ stbi_load(filePath.c_str(), &textureWidth, &textureHeight, &textureNrChannels, 0)};
    if (!textureData) {
        std::println("Failed to load texture");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB + RGB_A, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(textureData);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::bindTexture(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->m_id);
}

void Texture::unbindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}