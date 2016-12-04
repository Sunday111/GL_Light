#pragma once

#include <vector>
#include <memory>

#include "Base/Geom/BoundingBox.h"
#include "Base/Geom/VertexBlob.h"
#include "Base/Geom/IndexBlob.h"

//This class must be immutable
class MeshData
{
public:
        explicit MeshData(VertexBlobPtr vertData,
            const IndexBlobPtr& indices);

        explicit MeshData(VertexBlobPtr vertData,
            const IndexBlobPtr& indices,
            const BoundingBox3f& boundingBox);

        const VertexBlobPtr& GetVertexData() const { return m_vertexData; }
        const IndexBlobPtr& GetIndexData() const { return m_indexData; }
        const BoundingBox3f& GetBoundingBox() const { return m_boundingBox; }
private:
    VertexBlobPtr m_vertexData;
    IndexBlobPtr m_indexData;
    BoundingBox3f m_boundingBox;
};

using MeshDataPtr = std::shared_ptr<MeshData>;
using MeshDataConstPtr = std::shared_ptr<const MeshData>;