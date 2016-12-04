#include "Scene/MeshData.h"

MeshData::MeshData(VertexBlobPtr vertData,
    const IndexBlobPtr& indices) :
    m_vertexData(vertData),
    m_indexData(indices),
    m_boundingBox(BoundingBox3f::kInvalid)
{
    assert(m_vertexData != nullptr && m_indexData != nullptr);

    ArrayView<Vector3f> posView = m_vertexData->GetFieldView<VertexBlobField::Pos>();

    for (int idx : m_indexData->GetData())
    {
        m_boundingBox += posView[idx];
    }
}

MeshData::MeshData(VertexBlobPtr vertData,
    const IndexBlobPtr& indices,
    const BoundingBox3f& boundingBox) :
    m_vertexData(vertData),
    m_indexData(indices),
    m_boundingBox(boundingBox)
{
    assert(m_vertexData != nullptr && m_indexData != nullptr);
}