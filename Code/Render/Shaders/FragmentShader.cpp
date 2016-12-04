#include "Render/Shaders/FragmentShader.h"

FragmentShader::FragmentShader(const char* fileName,
    const char* additionalCode, std::ostream* logstream) :
    Shader(ShaderType::Fragment, fileName, additionalCode, logstream)
{}