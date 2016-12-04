#include "Scene/Lights/DirectionalLight.h"

DirectionalLight::DirectionalLight(
    const Vector3f& direction, const Vector3f& ambient,
    const Vector3f& diffuse, const Vector3f& specular) :
    LightSource(ambient, diffuse, specular),
    m_direction(direction)
{}