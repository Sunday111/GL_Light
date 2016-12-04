#include "Scene/Lights/LightSource.h"

LightSource::LightSource(const Vector3f& ambient,
    const Vector3f& diffuse, const Vector3f& specular) :
m_ambient(ambient),
m_diffuse(diffuse),
m_specular(specular)
{

}