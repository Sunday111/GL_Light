#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject(const VertexArrayObjectPtr& vao, const VertexBlobPtr& vertexBlob) :
    m_vao(vao),
    m_vertexBlob(vertexBlob)
{
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(vao->GetIdentifier());
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
        m_vertexBlob->TotalSize(),
        m_vertexBlob->Data(),
        GL_STATIC_DRAW);

    VertexBlobField fields = m_vertexBlob->GetFields();

    const int elemSize = XPointBlob::GetPointSize(fields);

    if ((fields & VertexBlobField::Pos) != VertexBlobField::Empty)
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, elemSize,
            //(GLvoid*)(XPointBlob::GetFieldOffset(VertexBlobField::Pos, fields)));
            (GLvoid*)(0 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
    }

    if ((fields & VertexBlobField::Norm) != VertexBlobField::Empty)
    {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, elemSize,
            //(GLvoid*)(XPointBlob::GetFieldOffset(VertexBlobField::Norm, fields)));
            (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    }

    if ((fields & VertexBlobField::TexCoords) != VertexBlobField::Empty)
    {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, elemSize,
            //(GLvoid*)(XPointBlob::GetFieldOffset(VertexBlobField::TexCoords, fields)));
            (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

VertexBufferObject::~VertexBufferObject()
{
    glDeleteBuffers(1, &m_VBO);
}

void VertexBufferObject::Draw()
{
    if (m_wireframeMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glBindVertexArray(m_vao->GetIdentifier());
    glDrawArrays(GL_TRIANGLES, 0, m_vertexBlob->Size());
    glBindVertexArray(0);
}