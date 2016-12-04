#pragma once

// GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GLEW/glew.h>
#include <memory>

class VertexArrayObject
{
public:
    VertexArrayObject();
    ~VertexArrayObject();

    GLuint GetIdentifier() const { return m_VAO; }

private:
    GLuint m_VAO;
};

using VertexArrayObjectPtr = std::shared_ptr<VertexArrayObject>;