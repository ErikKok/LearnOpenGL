#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

class Shader
{
public:
    Shader(const std::string& shaderPath);

    const unsigned int getId() const { return m_id; };

    void useShader() const;

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, GLint value) const;
    void setFloat(const std::string& name, GLfloat value) const;

    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, GLfloat x, GLfloat y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;

    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    int getLocation(const std::string& name) const;
    void checkCompileErrors(GLuint shader, std::string_view type) const;

    void getUniformLocation();

    unsigned int m_id;
};