#include <xmmintrin.h>

#include "Base/Geom/BoundingBox.h"
#include "Base/Geom/Quaternion.h"
#include "Base/Geom/Vector.h"

#include "Base/Geom/Transform.h"

const Transformf Transformf::kIdentity(1.0f);

void Transformf::SetRotation(const Quaternionf& q)
{
    const float qq[4] = {q.x, q.y, q.z, q.w};
    const float xd = qq[0] + qq[0];
    const float yd = qq[1] + qq[1];
    const float zd = qq[2] + qq[2];
    const float xx = qq[0] * xd;
    const float yx = qq[1] * xd;
    const float yy = qq[1] * yd;
    const float zx = qq[2] * xd;
    const float zy = qq[2] * yd;
    const float zz = qq[2] * zd;
    const float wx = qq[3] * xd;
    const float wy = qq[3] * yd;
    const float wz = qq[3] * zd;

    x0 = 1.0f - yy - zz;
    x1 = yx - wz;
    x2 = zx + wy;

    y0 = yx + wz;
    y1 = 1.0f - xx - zz;
    y2 = zy - wx;

    z0 = zx - wy;
    z1 = zy + wx;
    z2 = 1.0f - xx - yy;
}

void Transformf::Mul(const Transformf* a, const Transformf* b, Transformf* c)
{
    const float* const l = a->m;
    const float* const r = b->m;

    __m128 x = _mm_loadu_ps(l);
    __m128 y = _mm_loadu_ps(l + 4);
    __m128 z = _mm_loadu_ps(l + 8);
    __m128 t = _mm_loadu_ps(l + 12);

#define SERE_TR_MUL_COMP(C, R)\
    __m128 C = _mm_mul_ps(x, _mm_load_ps1(R));\
    C = _mm_add_ps(C, _mm_mul_ps(y, _mm_load_ps1(R + 1)));\
    C = _mm_add_ps(C, _mm_mul_ps(z, _mm_load_ps1(R + 2)));\
    C = _mm_add_ps(C, _mm_mul_ps(t, _mm_load_ps1(R + 3)))

    SERE_TR_MUL_COMP(ox, r);
    SERE_TR_MUL_COMP(oy, r + 4);
    SERE_TR_MUL_COMP(oz, r + 8);
    SERE_TR_MUL_COMP(ow, r + 12);

#undef SERE_TR_MUL_COMP

    float* const o = c->m;

    _mm_storeu_ps(o, ox);
    _mm_storeu_ps(o + 4, oy);
    _mm_storeu_ps(o + 8, oz);
    _mm_storeu_ps(o + 12, ow);
}

void Transformf::Transform(const Transformf* a, const Vector3f* b, Vector3f* c)
{
    // TODO: optimize with SSE
    const float* const t = a->m;
    const float v[3] = {b->x(), b->y(), b->z()};
    float res[3];

    res[0] = t[0] * v[0] + t[4] * v[1] + t[8] * v[2] + t[12];
    res[1] = t[1] * v[0] + t[5] * v[1] + t[9] * v[2] + t[13];
    res[2] = t[2] * v[0] + t[6] * v[1] + t[10] * v[2] + t[14];

    c->x() = res[0];
    c->y() = res[1];
    c->z() = res[2];
}

void Transformf::Transform(const Transformf* a, const BoundingBox3f* b, BoundingBox3f* c)
{
    const float* const t = a->m;

    Vector3f origin = b->min;

    a->Transform(a, &origin, &origin);

    const Vector3f diag = b->max - b->min;
    Vector3f x, y, z;

    x.x() = t[0] * diag.x() + t[12];
    x.y() = t[1] * diag.x() + t[13];
    x.z() = t[2] * diag.x() + t[14];

    y.x() = t[4] * diag.y() + t[12];
    y.y() = t[5] * diag.y() + t[13];
    y.z() = t[6] * diag.y() + t[14];

    z.x() = t[8] * diag.z() + t[12];
    z.y() = t[9] * diag.z() + t[13];
    z.z() = t[10] * diag.z() + t[14];

    *c = BoundingBox3f(origin);

    const Vector3f ox = origin + x;
    const Vector3f oy = origin + y;
    const Vector3f oz = origin + z;

    *c += ox;
    *c += oy;
    *c += oz;

    *c += ox + y;
    *c += ox + z;
    *c += oy + z;

    *c += ox + y + z;
}