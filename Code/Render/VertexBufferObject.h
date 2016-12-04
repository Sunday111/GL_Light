#pragma once

#include "Base/Geom/VertexBlob.h"
#include "Render/VertexArrayObject.h"

class VertexBufferObject
{
public:
    explicit VertexBufferObject(const VertexArrayObjectPtr& vao,
        const VertexBlobPtr& vertexBlob);
    ~VertexBufferObject();

    void Draw();

    const VertexArrayObjectPtr& GetVAO() const { return m_vao; }
    const VertexBlobPtr& GetVertexBlob() const { return m_vertexBlob; }

private:
    GLuint m_VBO;
    bool m_wireframeMode;
    VertexBlobPtr m_vertexBlob;
    VertexArrayObjectPtr m_vao;
};

using VertexBufferObjectPtr = std::shared_ptr<VertexBufferObject>;