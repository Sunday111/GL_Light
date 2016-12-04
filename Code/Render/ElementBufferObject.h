#pragma once

#include <vector>

#include "Render/VertexBufferObject.h"
#include "Base/Geom/IndexBlob.h"

class ElementBufferObject 
{
public:
    explicit ElementBufferObject(const VertexBufferObjectPtr& vbo,
        const IndexBlobPtr& indexBlob);

    ~ElementBufferObject();

    void Draw();

    const IndexBlobPtr& GetIndexBlob() const { return m_indexBlob; }

private:
    GLuint m_EBO;
    IndexBlobPtr m_indexBlob;
    VertexBufferObjectPtr m_vbo;
};

using ElementBufferObjectPtr = std::shared_ptr<ElementBufferObject>;