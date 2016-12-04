#pragma once

#include <memory>

#include "Base/EnumFlags.h"
#include "Base/ArrayView.h"
#include "Base/Geom/Vector.h"

// Possible fields per point
enum class VertexBlobField : uint16_t
{
    Empty = 0,
    // Position
    Pos = 1,
    // Normal
    Norm = 2,
    // Color
    Color = 4,
    // Texture coordinates
    TexCoords = 8,
    // Must be last entry
    _Last = 16
};

ENUM_FLAG_OPERATORS(VertexBlobField)

template<VertexBlobField> struct PointBlobFieldMeta {};
template<> struct PointBlobFieldMeta<VertexBlobField::Pos> { using Type = Vector3f; };
template<> struct PointBlobFieldMeta<VertexBlobField::Norm> { using Type = Vector3f; };
template<> struct PointBlobFieldMeta<VertexBlobField::Color> { using Type = Vector4f; };
template<> struct PointBlobFieldMeta<VertexBlobField::TexCoords> { using Type = Vector2f; };

namespace XPointBlob {

    int GetPointSize(VertexBlobField fields);
    int GetFieldOffset(VertexBlobField field, VertexBlobField fields);

}

class VertexBlob
{
    // Avoid unintended copying, can be enabled if there is good use case of it
    VertexBlob(const VertexBlob&) = delete;
    VertexBlob& operator=(const VertexBlob&) = delete;

public:
    VertexBlob();

    explicit VertexBlob(VertexBlobField fields, int size);

    VertexBlob(VertexBlob&& other);

    ~VertexBlob();

    // Points count
    int Size() const { return m_size; }

    int TotalSize() const;

    VertexBlobField GetFields() const { return m_fields; }

    template<VertexBlobField F>
    ArrayView<typename PointBlobFieldMeta<F>::Type> GetFieldView()
    {
        typedef typename PointBlobFieldMeta<F>::Type FieldType;

        if ((F & m_fields) == VertexBlobField::Empty || m_blob == nullptr)
        {
            return ArrayView<FieldType>();
        }

        uint8_t* const field = m_blob + XPointBlob::GetFieldOffset(F, m_fields);

        return ArrayView<FieldType>(reinterpret_cast<FieldType*>(field), m_size,
            XPointBlob::GetPointSize(m_fields));
    }

    template<VertexBlobField F>
    ArrayView<const typename PointBlobFieldMeta<F>::Type> GetFieldView() const
    {
        typedef const typename PointBlobFieldMeta<F>::Type FieldType;

        if ((F & m_fields) == VertexBlobField::Empty || m_blob == nullptr)
        {
            return ArrayView<FieldType>();
        }

        const uint8_t* const field = m_blob + XPointBlob::GetFieldOffset(F, m_fields);

        return ArrayView<FieldType>(reinterpret_cast<FieldType*>(field), m_size,
            XPointBlob::GetPointSize(m_fields));
    }

    VertexBlob& Swap(VertexBlob& other);

    VertexBlob& operator=(VertexBlob&& other);

    const uint8_t* Data() const { return m_blob; }

private:
    VertexBlobField m_fields;
    uint8_t* m_blob;
    int m_size;
};

using VertexBlobPtr = std::shared_ptr<VertexBlob>;
using VertexBlobConstPtr = std::shared_ptr<const VertexBlob>;