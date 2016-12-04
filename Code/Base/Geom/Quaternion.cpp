#include "Base/Geom/Quaternion.h"

const Quaternionf Quaternionf::kIdentity(Vector3f(0.0f, 0.0f, 0.0f), 0.0f);

Quaternionf::Quaternionf(const Vector3f& axis, float angle)
{
    const float halfAngle = angle * 0.5f;
    const float a_sin = std::sin(halfAngle);

    x = a_sin * axis.x();
    y = a_sin * axis.y();
    z = a_sin * axis.z();
    w = std::cos(halfAngle);
}

void Quaternionf::Mul(const Quaternionf* a, const Quaternionf* b, Quaternionf* c)
{
    const float l[4] = {a->x, a->y, a->z, a->w};
    const float r[4] = {b->x, b->y, b->z, b->w};
    float o[4];

    o[0] =  l[0] * r[3] + l[1] * r[2] - l[2] * r[1] + l[3] * r[0];
    o[1] = -l[0] * r[2] + l[1] * r[3] + l[2] * r[0] + l[3] * r[1];
    o[2] =  l[0] * r[1] - l[1] * r[0] + l[2] * r[3] + l[3] * r[2];
    o[3] = -l[0] * r[0] - l[1] * r[1] - l[2] * r[2] + l[3] * r[3];

    c->x = o[0];
    c->y = o[1];
    c->z = o[2];
    c->w = o[3];
}

void Quaternionf::Slerp(const Quaternionf* a, const Quaternionf* b, float t, Quaternionf* c)
{
    const float s[4] = {a->x, a->y, a->z, a->w};
    float d[4] = {b->x, b->y, b->z, b->w};
    float cos_o = s[0] * d[0] + s[1] * d[1] + s[2] * d[2] + s[3] * d[3];

    if(cos_o < 0.0f)
    {
        cos_o = -cos_o;
        d[0] = -d[0];
        d[1] = -d[1];
        d[2] = -d[2];
        d[3] = -d[3];
    }

    float scale[2];

    if(1.0f - cos_o > std::numeric_limits<float>::epsilon())
    {
        // slerp
        const float omega = std::acos(cos_o);
        const float sin_o = std::sin(omega);

        scale[0] = std::sin((1.0f - t) * omega) / sin_o;
        scale[1] = std::sin(t * omega) / sin_o;
    }
    else
    {
        // lerp
        scale[0] = 1.0f - t;
        scale[1] = t;
    }

    c->x = scale[0] * s[0] + scale[1] * d[0];
    c->y = scale[0] * s[1] + scale[1] * d[1];
    c->z = scale[0] * s[2] + scale[1] * d[2];
    c->w = scale[0] * s[3] + scale[1] * d[3];
}