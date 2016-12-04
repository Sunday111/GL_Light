#pragma once

#include "Render/Shaders/VertexShader.h"
#include "Render/Shaders/FragmentShader.h"

class ShaderProgram
{
public:
    explicit ShaderProgram(
        const VertexShaderPtr& vertexShader,
        const FragmentShaderPtr& fragmentShader,
        std::ostream* logstream = nullptr);

    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;

    bool Use() const;

    GLuint GetUniformLocation(const GLchar* uniformName) const;
    GLuint TryGetUniformLocation(const GLchar* uniformName) const;

    void RecompileShaderWithText(ShaderType type, const char* addtionalCode);

    const VertexShaderPtr& GetVertexShader() const { return m_vertexShader; }

protected:
    bool Link(std::ostream* logstream = nullptr);

private:
    const GLuint m_id;
    VertexShaderPtr m_vertexShader;
    FragmentShaderPtr m_fragmentShader;
};

using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;