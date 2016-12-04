#pragma once

#include "Base/EnumFlags.h"
#include "Base/Geom/Vector.h"
#include "Scene/Lights/LightSource.h"

class SpotLight : public LightSource
{
public:
    explicit SpotLight(float innerAngle, float outerAngle,
        const Vector3f& direction,
        const Vector3f& position, const Vector3f& ambient,
        const Vector3f& diffuse, const Vector3f& specular);

    const Vector3f& GetDirection() const { return m_direction; }
    void SetDirection(const Vector3f& value) { m_direction = value; }
    const Vector3f& GetPosition() const { return m_position; }
    void SetPosition(const Vector3f& value) { m_position = value; }

    void SetInnerAngle(float angle);
    void SetOuterAngle(float angle);
    float GetInnerAngle() const { return m_innerAngle; }
    float GetOuterAngle() const { return m_outerAngle; }
    float GetInnerAngleCos() const;
    float GetOuterAngleCos() const;


private:
    enum class UpdateFlag
    {
        Empty         = 0,
        InnerAngleCos = 1,
        OuterAngleCos = 2
    };

    ENUM_FLAG_OPERATORS_CLASS(UpdateFlag);

    float m_innerAngle;
    float m_outerAngle;
    Vector3f m_direction;
    Vector3f m_position;

    mutable UpdateFlag m_flag;
    mutable float m_innerAngleCos;
    mutable float m_outerAngleCos;
};

using SpotLightPtr = std::shared_ptr<SpotLight>;