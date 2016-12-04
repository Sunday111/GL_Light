#include "Scene/Lights/SpotLight.h"

SpotLight::SpotLight(float innerAngle,
    float outerAngle, const Vector3f& direction,
    const Vector3f& position, const Vector3f& ambient,
    const Vector3f& diffuse, const Vector3f& specular) :
    LightSource(ambient, diffuse, specular),
    m_innerAngle(innerAngle),
    m_outerAngle(outerAngle),
    m_direction(direction),
    m_position(position),
    m_flag(UpdateFlag::InnerAngleCos | UpdateFlag::OuterAngleCos)
{}

float SpotLight::GetInnerAngleCos() const
{
    if ((m_flag & UpdateFlag::InnerAngleCos) != UpdateFlag::Empty)
    {
        m_innerAngleCos = cos(m_innerAngle);
        m_flag &= (~UpdateFlag::InnerAngleCos);
    }

    return m_innerAngleCos;
}

float SpotLight::GetOuterAngleCos() const
{
    if ((m_flag & UpdateFlag::OuterAngleCos) != UpdateFlag::Empty)
    {
        m_outerAngleCos = cos(m_outerAngle);
        m_flag &= (~UpdateFlag::OuterAngleCos);
    }

    return m_outerAngleCos;
}

void SpotLight::SetInnerAngle(float angle)
{
    m_innerAngle = angle;
    m_flag |= UpdateFlag::InnerAngleCos;
}

void SpotLight::SetOuterAngle(float angle)
{
    m_outerAngle = angle;
    m_flag |= UpdateFlag::OuterAngleCos;
}