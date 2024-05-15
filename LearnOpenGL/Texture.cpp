#pragma once
#include "Global.h"
#include "Texture.h"

#include <glad/glad.h>
#include <STB/stb_image.h>

#include <array>
#include <print>
#include <string>
#include <vector>

#include <bitset>
#include <iostream>
#include <sstream>
#include <cstdint>

Texture::Texture(const std::string& filePath, bool convertToLinearSpace)
    :m_filePath{ filePath }
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    int textureNrChannels{};
    unsigned char* textureData{ stbi_load(filePath.c_str(), &m_width, &m_height, &textureNrChannels, 0) };
    if (!textureData)
        std::println("Failed to load texture");
    GLenum format{};
    GLenum internalFormat{};
    assert(textureNrChannels == 1 || textureNrChannels == 3 || textureNrChannels == 4 && "Image format not supported");
    if (textureNrChannels == 1) {  [[unlikely]]
        format = GL_RED;
        internalFormat = GL_RED;
    }
    else if (textureNrChannels == 3) { [[likely]]
        format = GL_RGB;
        internalFormat = GL_SRGB;
    }
    else if (textureNrChannels == 4) {
        format = GL_RGBA;
        internalFormat = GL_SRGB_ALPHA;
    }
    if (!convertToLinearSpace)
        internalFormat = format;
    Global::glCheckError();
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, textureData);  
    glBindTexture(GL_TEXTURE_2D, 0);

    // TODO when using immutable textures, how to deal with convertToLinearSpace?
    // https://stackoverflow.com/questions/65831143/srgb-conversion-opengl
    //glTextureStorage2D(m_id, 1, internalFormat, m_width, m_height);
    //glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, format, GL_UNSIGNED_BYTE, textureData);

    glGenerateTextureMipmap(m_id);
    stbi_image_free(textureData);
    Global::glCheckError();
    std::println("CREATE texture id: {}, filePath: {}", m_id, m_filePath);
}

// Creates single color, single pixel texture from a hex value:
Texture::Texture(uint32_t color)
    :m_singleColor{ color }
    ,m_type { textureType::singleColor }
    ,m_width{ 1 }
    ,m_height{ 1 }
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_singleColor);
    glBindTexture(GL_TEXTURE_2D, 0);

    Global::glCheckError();
    std::println("CREATE texture single color id: {}", m_id);
}

// Loads a cubeMap texture from 6 individual texture faces in order:
// +X (right), -X (left), +Y (top), -Y (bottom), +Z (front), -Z (back)
Texture::Texture(const std::vector<std::string>& faces)
    :m_type{ textureType::cubeMap }
    ,m_filePath{ faces[0] }
{
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_id);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    stbi_set_flip_vertically_on_load(false);
    int textureNrChannels{};
    for (unsigned int i = 0u; i < faces.size(); i++)
    {
        unsigned char* textureData{ stbi_load(faces[i].c_str(), &m_width, &m_height, &textureNrChannels, 0) };
        assert(textureNrChannels == 3 && "CubeMap not in RGB format!");
        if (!textureData)
            std::println("Failed to load texture {}", faces[i]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        stbi_image_free(textureData);
        Global::glCheckError();
    }
    std::println("CREATE cubeMap id: {}", m_id);
}

// Creates a depthMap:
Texture::Texture(textureType textureType, GLsizei width, GLsizei height)
    : m_type{ textureType::depthMap }
    , m_width{ width }
    , m_height{ height }
{
    assert(textureType == textureType::depthMap && "Wrong textureType for this constructor");

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    Global::glCheckError();
    std::println("CREATE texture depthMap id: {}", m_id);
}

Texture::~Texture()
{
        std::println("DELETE texture id: {}", m_id);
        glDeleteTextures(1, &m_id);
        Global::glCheckError();
}

void Texture::bindTexture(GLuint textureUnit)
{
    std::println("BIND texture id: {} | texture unit: {}", m_id, textureUnit);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    if (m_type == textureType::cubeMap) [[unlikely]]
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_id);
    else [[likely]]
        glBindTexture(GL_TEXTURE_2D, this->m_id);
    setBound(textureUnit);
    Global::glCheckError();
}

void Texture::unbindTexture()
{
    std::println("UNBIND texture id: {}", m_id);
    if (m_type == textureType::cubeMap) [[unlikely]]
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    else [[likely]]
        glBindTexture(GL_TEXTURE_2D, 0);
    setBound(-1);
    Global::glCheckError();
}

void Texture::activeTexture() const
{
    assert(m_type != textureType::cubeMap); 
    
    std::println("ACTIVE texture id: {} | texture unit: {}", m_id, m_boundTextureUnit);
    glActiveTexture(GL_TEXTURE0 + m_boundTextureUnit);
    Global::glCheckError();
}

//// Check if texture is bound:
//GLint returnData{};
//// activate proper texture unit (i) and check if texture is already bound
//glActiveTexture(GL_TEXTURE0 + i);
//glGetIntegerv(GL_TEXTURE_BINDING_2D, &returnData); // Get currently bound Texture
//unsigned int textureId{ texture.getId() };
//// Bind texture if unbound
//if (textureId != static_cast<unsigned int>(returnData))
//glBindTexture(GL_TEXTURE_2D, textureId);