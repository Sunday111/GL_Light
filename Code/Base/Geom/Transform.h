#pragma once

#include "Base/Geom/Vector.h"
#include "Base/Geom/BoundingBox.h"

class Quaternionf;

class Transformf
{
public:
    static const Transformf kIdentity;

    Transformf() {}

    // Scale transform
    explicit Transformf(float s) :
        x0(s), y0(0.0f), z0(0.0f), w0(0.0f),
        x1(0.0f), y1(s), z1(0.0f), w1(0.0f),
        x2(0.0f), y2(0.0f), z2(s), w2(0.0f),
        x3(0.0f), y3(0.0f), z3(0.0f), w3(1.0f)
    {
    }

    // Translation transform
    explicit Transformf(float x, float y, float z) :
        x0(1.0f), y0(0.0f), z0(0.0f), w0(0.0f),
        x1(0.0f), y1(1.0f), z1(0.0f), w1(0.0f),
        x2(0.0f), y2(0.0f), z2(1.0f), w2(0.0f),
        x3(x), y3(y), z3(z), w3(1.0f)
    {
    }

    // Rotation transform
    explicit Transformf(const Quaternionf& q)
    {
        SetRotation(q);

        w0 = w1 = w2 = 0.0f;
        x3 = y3 = z3 = 0.0f;
        w3 = 1.0f;
    }

    Transformf(const Transformf&) = default;

    void SetRotation(const Quaternionf& q);

    Transformf& operator=(const Transformf&) = default;

    Transformf operator*(const Transformf& t) const
    {
        Transformf t0;

        Mul(this, &t, &t0);

        return t0;
    }

    Transformf& operator*=(const Transformf& t)
    {
        Mul(this, &t, this);

        return *this;
    }

    Vector3f operator*(const Vector3f& v) const
    {
        Vector3f res;

        Transform(this, &v, &res);

        return res;
    }

    BoundingBox3f operator*(const BoundingBox3f& b) const
    {
        BoundingBox3f res;

        Transform(this, &b, &res);

        return res;
    }

    union
    {
        float m[16];
        struct
        {
            float x0, y0, z0, w0,
                  x1, y1, z1, w1,
                  x2, y2, z2, w2,
                  x3, y3, z3, w3;
        };
    };

private:
    // Matrix multiplication
    static void Mul(const Transformf* a, const Transformf* b, Transformf* c);

    // Vector transformation
    static void Transform(const Transformf* a, const Vector3f* b, Vector3f* c);

    // Bounding box transformation
    static void Transform(const Transformf* a, const BoundingBox3f* b, BoundingBox3f* c);
};