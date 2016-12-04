#include <cassert>

#include "Render/Shaders/ShaderProgram.h"

ShaderProgram::ShaderProgram(const VertexShaderPtr& vertexShader,
    const FragmentShaderPtr& fragmentShader, std::ostream* logstream) :
m_id(glCreateProgram()),
m_vertexShader(vertexShader),
m_fragmentShader(fragmentShader)
{
    if (m_vertexShader != nullptr)
    {
        glAttachShader(m_id, vertexShader->GetId());
    }

    if (m_fragmentShader != nullptr)
    {
        glAttachShader(m_id, m_fragmentShader->GetId());
    }

    const bool linkRes = Link(logstream);
    assert(linkRes);
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_id);
}

bool ShaderProgram::Use() const
{
    glUseProgram(m_id);
    return true;
}

GLuint ShaderProgram::GetUniformLocation(const GLchar* uniformName) const
{
    const GLuint result = glGetUniformLocation(m_id, uniformName);
    assert(result != -1);
    return result;
}

GLuint ShaderProgram::TryGetUniformLocation(const GLchar* uniformName) const
{
    const GLuint result = glGetUniformLocation(m_id, uniformName);
    return result;
}

void ShaderProgram::RecompileShaderWithText(ShaderType type, const char* addtionalCode)
{
    switch (type)
    {
    case ShaderType::Vertex:
        m_vertexShader->RecompileShaderWithCode(addtionalCode);
        break;
    case ShaderType::Fragment:
        m_fragmentShader->RecompileShaderWithCode(addtionalCode);
        break;
    default:
        assert(!"Not implemented here");
        break;
    }
}

bool ShaderProgram::Link(std::ostream* logstream)
{
    glLinkProgram(m_id);

    GLint success;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);

    if (!success) {
        if (logstream != nullptr)
        {
            GLchar infoLog[512];
            glGetProgramInfoLog(m_id, 512, NULL, infoLog);
            *logstream << "ERROR::SHADER_PROGRAM::" << m_id
                << "::LINK_FAILED\n" << infoLog << std::endl;
        }

        return false;
    }

    return true;
}