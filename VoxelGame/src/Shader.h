#pragma once

#include <glm/mat4x4.hpp>

class Shader
{
public:
    /* Public methods */
    Shader(const std::string& vertexFile, const std::string& fragmentFile);

    void Use() const;
    void Stop() const;

    void SetVec3(const std::string& name, float v0, float v1, float v2);
    void SetMat4(const std::string& name, const glm::mat4& matrix);

private:
    /* Private methods */
    static std::string ReadFile(const std::string& path);
    static unsigned int CompileShader(const char* source, unsigned int type);
    static unsigned int LinkProgram(unsigned int vertexShader, unsigned int fragmentShader);

    int GetUniformLocation(const std::string& name);
private:
    /* Private members */
    unsigned int m_id;
    std::map<std::string, int> m_uniformLocations;
};
