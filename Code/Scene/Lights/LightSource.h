#pragma once

#include <memory>
#include "Base/Geom/Vector.h"

/* Base class for light sources, which contains
 * light ambient, diffuse and specular color
 */
class LightSource
{
public:
    const Vector3f& GetAmbient() const { return m_ambient; }
    const Vector3f& GetDiffuse() const { return m_diffuse; }
    const Vector3f& GetSpecular() const { return m_specular; }

    void SetAmbient(const Vector3f& value) { m_ambient = value; }
    void SetDiffuse(const Vector3f& value) { m_diffuse = value; }
    void SetSpecular(const Vector3f& value) { m_specular = value; }

protected:
    explicit LightSource(const Vector3f& ambient,
        const Vector3f& diffuse, const Vector3f& specular);
    LightSource(const LightSource&) = default;
    LightSource& operator= (const LightSource&) = default;

private:
    Vector3f m_ambient;
    Vector3f m_diffuse;
    Vector3f m_specular;
};