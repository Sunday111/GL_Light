#include "Render/VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
{
    glGenVertexArrays(1, &m_VAO);
}

VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &m_VAO);
}