#include "Render/Shaders/VertexShader.h"

VertexShader::VertexShader(const char* fileName,
    const char* additionalCode, std::ostream* logstream) :
    Shader(ShaderType::Vertex, fileName, additionalCode, logstream)
{}