#pragma once

#include <limits>
#include "Base/Geom/Vector.h"

class Quaternionf
{
public:
    static const Quaternionf kIdentity;

    Quaternionf() {}

    Quaternionf(const Vector3f& axis, float angle);

    Quaternionf(const Quaternionf&) = default;

    float LengthSq() const { return x * x + y * y + z * z + w * w; }

    float Length() const { return std::sqrt(x * x + y * y + z * z + w * w); }

    bool IsNull() const
    {
        return std::abs(x) < std::numeric_limits<float>::epsilon() &&
               std::abs(y) < std::numeric_limits<float>::epsilon() &&
               std::abs(z) < std::numeric_limits<float>::epsilon() &&
               std::abs(w) < std::numeric_limits<float>::epsilon();
    }

    // Spherical linear interpolation between two quaternions
    Quaternionf Slerp(const Quaternionf& q, float t) const
    {
        Quaternionf c;

        Slerp(this, &q, t, &c);

        return c;
    }

    Quaternionf& operator=(const Quaternionf&) = default;

    Quaternionf operator*(const Quaternionf& q) const
    {
        Quaternionf c;

        Mul(this, &q, &c);

        return c;
    }

    Quaternionf& operator*=(const Quaternionf& q)
    {
        Mul(this, &q, this);

        return *this;
    }

    union
    {
        float xyzw[4];

        struct
        {
            float x, y, z, w;
        };
    };

private:
    static void Mul(const Quaternionf* a, const Quaternionf* b, Quaternionf* c);
    static void Slerp(const Quaternionf* a, const Quaternionf* b, float t, Quaternionf* c);
};
