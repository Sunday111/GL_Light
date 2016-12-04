#pragma once

#include <memory>
#include "Render/Shaders/Shader.h"

class VertexShader :
    public Shader
{
    public:
        explicit VertexShader(const char* fileName,
            const char* additionalCode = nullptr,
            std::ostream* logstream = nullptr);
};

using VertexShaderPtr = std::shared_ptr<VertexShader>;