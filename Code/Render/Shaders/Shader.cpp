#include <cassert>
#include <filesystem>
#include <fstream>
#include <vector>

#include "Render/Shaders/Shader.h"

namespace
{
    static GLenum GetGLEnum(ShaderType type)
    {
        switch (type)
        {
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
        default:
            assert(!"Implement here");
            return 0;
        }
    }

    static const GLchar* GetShaderTypeName(ShaderType type)
    {
        switch (type)
        {
        case ShaderType::Vertex:
            return "Vertex";
        case ShaderType::Fragment:
            return "Fragment";
        default:
            assert(!"Implement here");
            return nullptr;
        }
    }
}

Shader::Shader(ShaderType type, const char* fileName,
    const char* additionalCode, std::ostream* logstream) :
m_type(type),
m_id(glCreateShader(GetGLEnum(type)))
{
    const bool compileResult = Compile(fileName, additionalCode, logstream);
    assert(compileResult);
}

Shader::~Shader()
{
    glDeleteShader(m_id);
}

bool Shader::RecompileShaderWithCode(const char* code,
    std::ostream* logstream)
{
    assert(code != nullptr);

    const char * codePointers[2]
    {
        code,
        m_code.data()
    };

    glShaderSource(m_id, 2, codePointers, NULL);
    glCompileShader(m_id);

    GLint success;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        if (logstream != nullptr)
        {
            GLchar infoLog[512];
            glGetShaderInfoLog(m_id, 512, NULL, infoLog);
            *logstream << "ERROR::SHADER::" << GetShaderTypeName(m_type)
                << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        return false;
    }

    return true;
}

bool Shader::Compile(const char* fileName, const char* additionalCode, std::ostream* logstream)
{
    using namespace std::tr2::sys;

    if (!exists(fileName))
    {
        if (logstream != nullptr)
        {
            *logstream << "ERROR::SHADER::" << GetShaderTypeName(m_type)
                << std::endl << "file not found" << std::endl;
        }

        return false;
    }

    std::ifstream is(fileName, std::ifstream::binary);
    if (is)
    {
        // get length of file :
        is.seekg(0, is.end);
        size_t length = static_cast<size_t>(is.tellg());
        is.seekg(0, is.beg);

        m_code.resize(length + 1);
        if (!is.read(&m_code[0], length) && logstream != nullptr)
        {
            *logstream << "WARNING::SHADER::"
                << GetShaderTypeName(m_type)
                << std::endl << "only " << is.gcount()
                << " could be read" << std::endl;
        }

        is.close();
    }

    //Compile vertex shader
    if (additionalCode == nullptr)
    {
        const GLchar* shaderCodePtr = m_code.data();
        glShaderSource(m_id, 1, &shaderCodePtr, NULL);
    }
    else
    {
        const char * codePointers[2]
        {
            additionalCode,
            m_code.data()
        };

        glShaderSource(m_id, 2, codePointers, NULL);
    }

    glCompileShader(m_id);
    GLint success;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        if (logstream != nullptr)
        {
            GLchar infoLog[512];
            glGetShaderInfoLog(m_id, 512, NULL, infoLog);
            *logstream << "ERROR::SHADER::" << GetShaderTypeName(m_type)
                << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        return false;
    }

    return true;
}