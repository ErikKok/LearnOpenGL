#pragma once
#include "Global.h"
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

    glGenTextures(1, &m_Id);
    glBindTexture(GL_TEXTURE_2D, m_Id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* textureData{ stbi_load(filePath.c_str(), &textureWidth, &textureHeight, &textureNrChannels, 0) };
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
    glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(textureData);
    Global::glCheckError();
    std::println("CREATE texture id: {}", m_Id);
}

Texture::~Texture()
{
    std::println("DELETE ***DISABLED*** texture id: {}", m_Id);
    //glDeleteTextures(1, &m_id); // TODO
    Global::glCheckError();
}

void Texture::bindTexture(unsigned int textureUnit) const
{
    //std::println("BIND texture id: {}", m_Id);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, this->m_Id);
    Global::glCheckError();
}

void Texture::unbindTexture() const
{
    std::println("UNBIND texture id: {}", m_Id);
    glBindTexture(GL_TEXTURE_2D, 0);
    Global::glCheckError();
}