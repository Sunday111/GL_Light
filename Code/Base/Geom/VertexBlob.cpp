#include "Base/Geom/VertexBlob.h"

#include <new>
#include <utility>

namespace XPointBlob {

    static int GetFieldSize(VertexBlobField field)
    {
        switch (field)
        {
        case VertexBlobField::Pos:
            return sizeof(PointBlobFieldMeta<VertexBlobField::Pos>::Type);
        case VertexBlobField::Norm:
            return sizeof(PointBlobFieldMeta<VertexBlobField::Norm>::Type);
        case VertexBlobField::Color:
            return sizeof(PointBlobFieldMeta<VertexBlobField::Color>::Type);
        case VertexBlobField::TexCoords:
            return sizeof(PointBlobFieldMeta<VertexBlobField::TexCoords>::Type);
        default:
            assert(false);
            return 0;
        }
    }

    int GetPointSize(VertexBlobField fields)
    {
        int pointSize = 0;

        for (VertexBlobField i = static_cast<VertexBlobField>(1); i < VertexBlobField::_Last; i <<= 1)
        {
            if ((fields & i) != VertexBlobField::Empty)
            {
                pointSize += GetFieldSize(i);
            }
        }

        return pointSize;
    }

    int GetFieldOffset(VertexBlobField field, VertexBlobField fields)
    {
        int fieldOffset = 0;

        for (VertexBlobField i = field >> 1; i > VertexBlobField::Empty; i >>= 1)
        {
            if ((fields & i) != VertexBlobField::Empty)
            {
                fieldOffset += GetFieldSize(i);
            }
        }

        return fieldOffset;
    }

}

VertexBlob::VertexBlob() :
    m_fields(VertexBlobField::Empty),
    m_blob(nullptr),
    m_size(0)
{
}

VertexBlob::VertexBlob(VertexBlobField fields, int size) :
    m_fields(fields),
    m_blob(nullptr),
    m_size(0)
{
    const size_t blobSize = size > 0 ?
        static_cast<size_t>(size) * XPointBlob::GetPointSize(fields) : 0;

    if (blobSize > 0)
    {
        uint8_t* const blob = new (std::nothrow) uint8_t[blobSize];

        if (blob != nullptr)
        {
            m_blob = blob;
            m_size = size;
        }
    }
}

VertexBlob::VertexBlob(VertexBlob&& other) :
    m_fields(std::move(other.m_fields)),
    m_blob(std::move(other.m_blob)),
    m_size(std::move(other.m_size))
{
    other.m_fields = VertexBlobField::Empty;
    other.m_blob = nullptr;
    other.m_size = 0;
}

VertexBlob::~VertexBlob()
{
    delete[] m_blob;
}

int VertexBlob::TotalSize() const
{
    return XPointBlob::GetPointSize(m_fields) * Size();
}

VertexBlob& VertexBlob::Swap(VertexBlob& other)
{
    std::swap(m_fields, other.m_fields);
    std::swap(m_blob, other.m_blob);
    std::swap(m_size, other.m_size);

    return *this;
}

VertexBlob& VertexBlob::operator=(VertexBlob&& other)
{
    if (this != &other)
    {
        delete[] m_blob;

        m_fields = std::move(other.m_fields);
        m_blob = std::move(other.m_blob);
        m_size = std::move(other.m_size);

        other.m_fields = VertexBlobField::Empty;
        other.m_blob = nullptr;
        other.m_size = 0;
    }

    return *this;
}