#pragma once

#include <memory>
#include "Render/Shaders/Shader.h"

class FragmentShader :
    public Shader
{
public:
    explicit FragmentShader(const char* fileName,
        const char* additionalCode = nullptr,
        std::ostream* logstream = nullptr);
};

using FragmentShaderPtr = std::shared_ptr<FragmentShader>;