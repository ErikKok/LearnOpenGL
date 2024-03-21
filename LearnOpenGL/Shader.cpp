#pragma once
#include "Shader.h"



#include <print>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& shaderPath)
{
    std::ifstream stream(shaderPath);

    std::string shaderCode{};
    std::ifstream shaderFile{};
    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        shaderFile.open(shaderPath);
        std::stringstream shaderStream{};
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // convert stream into string
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure& error)
    {
        std::println("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {}", error.what());
    }

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
    };

    std::string line{};
    std::stringstream ss[2];
    ShaderType type{ ShaderType::NONE };
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    std::string vertexCode{ ss[0].str() };
    std::string fragmentCode{ ss[1].str() };

    // Je kan dit ook in deze struct opslaan (returnen), en in een volgende shaderCompile functie inladen
    // Zo kan je verschillende vertex- en fragmentshaders combineren
    //struct ShaderProgramSource {
    //    std::string VertexSource{};
    //    std::string FragmentSource{};
    //};

    const GLchar* vShaderCode{ vertexCode.c_str() };
    const GLchar* fShaderCode{ fragmentCode.c_str() };

    // vertex shader
    GLuint vertex{ glCreateShader(GL_VERTEX_SHADER) };
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    GLuint fragment{ glCreateShader(GL_FRAGMENT_SHADER) };
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    checkCompileErrors(m_id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    //Actually, you're still computing the hash twice. His code does it three times. What you could do if you have C++17 is:
    //    auto uniform{ cache.try_emplace(name, 0) };

    //if (uniform.second)
    //    return (uniform.first->second = glGetUniformLocation(m_ID, name.c_str()));
    //else
    //    return uniform.first->second;
    //If you are restricted to C++11, I fall short of ideas to implement it with standard things.I guess you would have to implement your own version of try_emplace.

    //    Why didn't he realise that he can avoid the whole mutable situation by caching all uniforms at the creation of the shader?
}

//{
//    // 1. retrieve the vertex/fragment source code from filePath
//    std::string vertexCode{};
//    std::string fragmentCode{};
//    std::ifstream vShaderFile{};
//    std::ifstream fShaderFile{};
//    // ensure ifstream objects can throw exceptions:
//    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//    try
//    {
//        // open files
//        vShaderFile.open(vertexPath);
//        fShaderFile.open(fragmentPath);
//        std::stringstream vShaderStream{};
//        std::stringstream fShaderStream{};
//        // read file's buffer contents into streams
//        vShaderStream << vShaderFile.rdbuf();
//        fShaderStream << fShaderFile.rdbuf();
//        // close file handlers
//        vShaderFile.close();
//        fShaderFile.close();
//        // convert stream into string
//        vertexCode = vShaderStream.str();
//        fragmentCode = fShaderStream.str();
//    }
//    catch (std::ifstream::failure& error)
//    {
//        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << error.what() << '\n';
//    }
//    const GLchar* vShaderCode{ vertexCode.c_str() };
//    const GLchar* fShaderCode{ fragmentCode.c_str() };
//    // 2. compile shaders
//    // vertex shader
//    GLuint vertex{ glCreateShader(GL_VERTEX_SHADER) };
//    glShaderSource(vertex, 1, &vShaderCode, nullptr);
//    glCompileShader(vertex);
//    checkCompileErrors(vertex, "VERTEX");
//    // fragment Shader
//    GLuint fragment{ glCreateShader(GL_FRAGMENT_SHADER) };
//    glShaderSource(fragment, 1, &fShaderCode, nullptr);
//    glCompileShader(fragment);
//    checkCompileErrors(fragment, "FRAGMENT");
//    // shader Program
//    m_id = glCreateProgram();
//    glAttachShader(m_id, vertex);
//    glAttachShader(m_id, fragment);
//    glLinkProgram(m_id);
//    checkCompileErrors(m_id, "PROGRAM");
//    // delete the shaders as they're linked into our program now and no longer necessary
//    glDeleteShader(vertex);
//    glDeleteShader(fragment);
//}

void Shader::useShader()
{
    glUseProgram(m_id);
    Global::glCheckError();
}

int Shader::getLocation(const std::string& name) const{
    int location{ glGetUniformLocation(m_id, name.c_str()) };

    if (location == -1)
        std::println("ERROR setting uniform value: \"{}\" does not correspond to active uniform, starts with gl_ or is associated with an atomic counter or a named uniform block! {}", name, location);
    Global::glCheckError();
    return location;
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(Shader::getLocation(name), (GLint)value);
}

void Shader::setInt(const std::string& name, GLint value) const
{
    glUniform1i(Shader::getLocation(name), value);
}

void Shader::setFloat(const std::string& name, GLfloat value) const
{
    glUniform1f(Shader::getLocation(name), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(Shader::getLocation(name), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, GLfloat x, GLfloat y) const
{
    glUniform2f(Shader::getLocation(name), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(Shader::getLocation(name), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const
{
    glUniform3f(Shader::getLocation(name), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(Shader::getLocation(name), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
{
    glUniform4f(Shader::getLocation(name), x, y, z, w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(Shader::getLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(Shader::getLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(Shader::getLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(GLuint shader, std::string_view type)
{
    int success{};
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::println("ERROR::SHADER_COMPILATION_ERROR of type: {}", type);
            std::println("{}", infoLog);
            std::println("------------------------------------------------------ ");
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::println("ERROR::PROGRAM_LINKING_ERROR of type: {}", type);
            std::println("{}", infoLog);
            std::println("------------------------------------------------------ ");
        }
    }
    Global::glCheckError();
}