#pragma once
#include "Texture.h"

#include <glad/glad.h>
#include <STB/stb_image.h>

#include <print>
#include <string>

Texture::Texture(const std::string& filePath)
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
    GLenum format{};
    if (textureNrChannels == 1)
        format = GL_RED;
    else if (textureNrChannels == 3)
        format = GL_RGB;
    else if (textureNrChannels == 4)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(textureData);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::bindTexture(unsigned int textureUnit) const
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, this->m_id);
}

void Texture::unbindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}