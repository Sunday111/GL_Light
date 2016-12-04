#pragma once

#include "Base/Geom/Vector.h"
#include "Scene/Lights/LightSource.h"

class DirectionalLight : public LightSource
{
public:
    explicit DirectionalLight(
        const Vector3f& direction,
        const Vector3f& ambient,
        const Vector3f& diffuse,
        const Vector3f& specular);

    const Vector3f& GetDirection() const { return m_direction; }

private:
    Vector3f m_direction;
};

using DirectionalLightPtr = std::shared_ptr<DirectionalLight>;