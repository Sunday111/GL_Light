#pragma once

// GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GLEW/glew.h>

#include <ostream>

#include "Render/Shaders/ShaderType.h"

//This is the base class for shader
class Shader
{
public:
    Shader(const Shader&) = delete;
    ~Shader();

    ShaderType GetShaderType() const { return m_type; }
    GLuint GetId() const { return m_id; }

    bool RecompileShaderWithCode(const char* code,
        std::ostream* logstream = nullptr);

protected:
    explicit Shader(ShaderType type, const char* fileName,
        const char* additionalCode = nullptr,
        std::ostream* logstream = nullptr);

    bool Compile(const char* fileName,
        const char* additionalCode = nullptr,
        std::ostream* logstream = nullptr);

private:
    const ShaderType m_type;
    const GLuint m_id;
    std::string m_code;
};