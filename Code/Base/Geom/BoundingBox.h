#pragma once

#include "Base/ArrayView.h"
#include "Base/Geom/Vector.h"

class BoundingBox3f
{
public:
    static const BoundingBox3f kInvalid;

    BoundingBox3f() {}

    explicit BoundingBox3f(const Vector3f& v) :
        min(v), max(v) {}

    explicit BoundingBox3f(const Vector3f& min_, const Vector3f& max_) :
        min(min_), max(max_)
    {
        assert(min.x() <= max.x() && min.y() <= max.y() && min.z() <= max.z());
    }

    BoundingBox3f(const BoundingBox3f&) = default;

    bool IsValid() const { return min.x() <= max.x(); }

    void Enlarge(float val)
    {
        assert(val > -std::numeric_limits<float>::epsilon());

        min.x() -= val;
        min.y() -= val;
        min.z() -= val;

        max.x() += val;
        max.y() += val;
        max.z() += val;
    }

    BoundingBox3f Enlarged(float val) const
    {
        assert(val > -std::numeric_limits<float>::epsilon());

        BoundingBox3f res(*this);

        res.min.x() -= val;
        res.min.y() -= val;
        res.min.z() -= val;

        res.max.x() += val;
        res.max.y() += val;
        res.max.z() += val;

        return res;
    }

    BoundingBox3f& operator+=(const BoundingBox3f& b)
    {
        *this += b.min;
        *this += b.max;

        return *this;
    }

    BoundingBox3f& operator+=(const Vector3f& v)
    {
        if (min.x() > v.x()) { min.x() = v.x(); }
        if (min.y() > v.y()) { min.y() = v.y(); }
        if (min.z() > v.z()) { min.z() = v.z(); }

        if (max.x() < v.x()) { max.x() = v.x(); }
        if (max.y() < v.y()) { max.y() = v.y(); }
        if (max.z() < v.z()) { max.z() = v.z(); }

        return *this;
    }

    BoundingBox3f& operator+=(const ArrayView<Vector3f>& va)
    {
        const int vaSize = va.size();

        for (int i = 0; i < vaSize; ++i)
        {
            const Vector3f& v = va[i];

            if (min.x() > v.x()) { min.x() = v.x(); }
            if (min.y() > v.y()) { min.y() = v.y(); }
            if (min.z() > v.z()) { min.z() = v.z(); }

            if (max.x() < v.x()) { max.x() = v.x(); }
            if (max.y() < v.y()) { max.y() = v.y(); }
            if (max.z() < v.z()) { max.z() = v.z(); }
        }

        return *this;
    }

    BoundingBox3f& operator+=(const ArrayView<const Vector3f>& va)
    {
        const int vaSize = va.size();

        for (int i = 0; i < vaSize; ++i)
        {
            const Vector3f& v = va[i];

            if (min.x() > v.x()) { min.x() = v.x(); }
            if (min.y() > v.y()) { min.y() = v.y(); }
            if (min.z() > v.z()) { min.z() = v.z(); }

            if (max.x() < v.x()) { max.x() = v.x(); }
            if (max.y() < v.y()) { max.y() = v.y(); }
            if (max.z() < v.z()) { max.z() = v.z(); }
        }

        return *this;
    }

    BoundingBox3f& operator=(const BoundingBox3f&) = default;

    Vector3f min;
    Vector3f max;

private:
    explicit BoundingBox3f(const Vector3f& min_, const Vector3f& max_, bool) :
        min(min_), max(max_)
    {
    }
};
