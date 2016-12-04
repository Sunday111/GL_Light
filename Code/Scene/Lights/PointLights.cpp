#include "Scene/Lights/PointLight.h"

Attenuation::Attenuation(float distance) :
    m_distance(distance),
    m_needRecompute(true)
{}

void Attenuation::SetDistance(float value)
{
    m_distance = value;
    m_needRecompute = true;
}

void Attenuation::DefaultConstantDistnaceDependency(float distance,
    float* constant, float* linear, float* quadratic)
{
    const int tableSize = 12;

    static const float distances[tableSize]
    {
        7.f, 13.f, 20.f, 32.f, 50.f, 65.f, 100.f, 160.f, 200.f, 325.f, 600.f, 3250.f
    };

    static const float linears[tableSize]
    {
        0.7f, 0.35f, 0.22f, 0.14f, 0.09f, 0.07f, 0.045f, 0.027f, 0.022f, 0.014f, 0.007f, 0.0014f
    };

    static const float quadratics[tableSize]
    {
        1.8f, 0.44f, 0.20f, 0.07f, 0.032f, 0.017f, 0.0075f, 0.0028f, 0.0019f, 0.0007f, 0.0002f, 0.000007f
    };

    if (constant != nullptr)
    {
        *constant = 1.f;
    }

    int range[2]{ 0, 0 };

    //Find interpolation indices
    for (int idx = tableSize - 1; idx > 0; --idx)
    {
        if (distance > distances[idx])
        {
            range[0] = idx;
            range[1] = range[0] + (idx == tableSize - 1 ? 0 : 1);
            break;
        }
    }

    if (range[0] == range[1])
    {
        if (linear != nullptr)
        {
            *linear = linears[range[0]];
        }

        if (quadratic != nullptr)
        {
            *quadratic = quadratics[range[0]];
        }

        return;
    }

    const float distanceRange = distances[range[1]] - distances[range[0]];
    const float distanceDelta = distance - distances[range[0]];
    const float portion = distanceDelta / distanceRange;

    if (linear != nullptr)
    {
        const float linearRange = linears[range[1]] - linears[range[0]];
        *linear = linears[range[0]] + linearRange * portion;
    }

    if (quadratic != nullptr)
    {
        const float quadraticRange = quadratics[range[1]] - quadratics[range[0]];
        *quadratic = quadratics[range[0]] + quadraticRange * portion;
    }
}

void Attenuation::RecomputeRates() const
{
    m_needRecompute = false;
    DefaultConstantDistnaceDependency(m_distance, &m_constant, &m_linear, &m_quadratic);
}

float Attenuation::GetConstantRate() const
{
    if (m_needRecompute)
    {
        RecomputeRates();
    }

    return m_constant;
}

float Attenuation::GetLinearRate() const
{
    if (m_needRecompute)
    {
        RecomputeRates();
    }

    return m_linear;
}

float Attenuation::GetQuadraticRate() const
{
    if (m_needRecompute)
    {
        RecomputeRates();
    }

    return m_quadratic;
}

PointLight::PointLight(const Vector3f& position, const Vector3f& ambient,
    const Vector3f& diffuse, const Vector3f& specular, float distance) :
LightSource(ambient, diffuse, specular),
m_position(position),
m_attenuation(distance)
{}