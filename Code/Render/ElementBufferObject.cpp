#include "Render/ElementBufferObject.h"

ElementBufferObject::ElementBufferObject(const VertexBufferObjectPtr& vbo,
    const IndexBlobPtr& indexBlob) :
    m_indexBlob(indexBlob),
    m_vbo(vbo)
{
    glBindVertexArray(m_vbo->GetVAO()->GetIdentifier());
    glGenBuffers(1, &m_EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(int) * m_indexBlob->GetData().size(),
        m_indexBlob->GetData().data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ElementBufferObject::~ElementBufferObject()
{
    glDeleteBuffers(1, &m_EBO);
}

void ElementBufferObject::Draw()
{
    glBindVertexArray(m_vbo->GetVAO()->GetIdentifier());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glDrawElements(GL_TRIANGLES, 
        static_cast<GLsizei>(m_indexBlob->GetData().size()),
        GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}