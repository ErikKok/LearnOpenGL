#pragma once
#include "Global.h"
#include "Texture.h"

#include <glad/glad.h>
#include <STB/stb_image.h>

#include <print>
#include <string>

Texture::Texture(const std::string& filePath)
    :m_filePath{ filePath }
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    int textureNrChannels{};
    unsigned char* textureData{ stbi_load(filePath.c_str(), &m_width, &m_height, &textureNrChannels, 0) };
    if (!textureData) {
        std::println("Failed to load texture");
    }
    GLenum format{};
    assert(textureNrChannels == 1 || textureNrChannels == 3 || textureNrChannels == 4 && "Image format not supported");
    if (textureNrChannels == 1) [[unlikely]]
        format = GL_RED;
    else if (textureNrChannels == 3) [[likely]]
        format = GL_RGB;
    else if (textureNrChannels == 4)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, 0); // TODO wel/niet gelijk un!!binden?
    stbi_image_free(textureData);
    Global::glCheckError();
    std::println("CREATE texture id: {}", m_id);
}

Texture::~Texture() // TODO - hacky... use smart pointers I guess? Wordt niet meer gebruikt nu!?
{
    std::println("************************************** ", m_id);
    if (m_type == "moved") {
        std::println("DELETE texture - Texture object IS deleted, but texture itself NOT! Original texture has been moved to another object!");
    }
    else {
        std::println("************************************** DELETE texture id: {}", m_id);
        glDeleteTextures(1, &m_id);
        Global::glCheckError();
    }
}

void Texture::bindTexture(unsigned int textureUnit) const
{
    //std::println("BIND texture id: {} | texture unit: {}", m_id, textureUnit);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, this->m_id);
    Global::glCheckError();
}

void Texture::unbindTexture() const
{
    std::println("UNBIND texture id: {}", m_id);
    glBindTexture(GL_TEXTURE_2D, 0);
    Global::glCheckError();
}