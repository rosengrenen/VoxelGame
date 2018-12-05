#pragma once

class Shader
{
public:
    /* Public methods */
    Shader(const std::string& vertexFile, const std::string& fragmentFile);
    void Use() const;
    void Stop() const;

private:
    /* Private methods */
    static std::string ReadFile(const std::string& path);
    static unsigned int CompileShader(const char* source, unsigned int type);
    static unsigned int LinkProgram(unsigned int vertexShader, unsigned int fragmentShader);

private:
    /* Private members */
    unsigned int m_id;
};
