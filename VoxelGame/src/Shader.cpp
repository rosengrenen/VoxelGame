#include "pch.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile)
{
    std::string vertexSource = ReadFile(vertexFile);
    std::string fragmentSource = ReadFile(fragmentFile);

    unsigned int vertex = CompileShader(vertexSource.c_str(), GL_VERTEX_SHADER);
    unsigned int fragment = CompileShader(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

    m_id = LinkProgram(vertex, fragment);
}

void Shader::Use() const
{
    glUseProgram(m_id);
}

void Shader::Stop() const
{
    glUseProgram(0);
}

void Shader::SetVec3(const std::string& name, float v0, float v1, float v2)
{
    Use();
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& matrix)
{
    Use();
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string Shader::ReadFile(const std::string& path)
{
    std::ifstream file(path);
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();

    std::string content(size, ' ');
    file.seekg(0);
    file.read(content.data(), size);

    return content;
}

unsigned int Shader::CompileShader(const char* source, unsigned type)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE)
    {
        char log[1024];
        int logLength;
        glGetShaderInfoLog(shader, 1024, &logLength, log);
        LOG_ERROR("Shader compile error: {}", log);
    }

    return shader;
}

unsigned int Shader::LinkProgram(unsigned vertexShader, unsigned fragmentShader)
{
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result != GL_TRUE)
    {
        char log[1024];
        int logLength;
        glGetProgramInfoLog(program, 1024, &logLength, log);
        LOG_ERROR("Shader program link error: {}", log);
    }

    return program;
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_uniformLocations.find(name) == m_uniformLocations.end())
    {
        int location = glGetUniformLocation(m_id, name.c_str());
        m_uniformLocations[name] = location;
        if (location == -1)
        {
            LOG_ERROR("Uniform '{}' not found", location);
        }
    }
    return m_uniformLocations[name];
}
