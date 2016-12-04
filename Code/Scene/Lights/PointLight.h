#pragma once

#include "Scene/Lights/LightSource.h"

class Attenuation
{
public:
    explicit Attenuation(float distance);

    void SetDistance(float value);
    float GetDistance() const { return m_distance; }

    float GetConstantRate() const;
    float GetLinearRate() const;
    float GetQuadraticRate() const;

private:
    static void DefaultConstantDistnaceDependency(float distance,
        float* constant, float* linear, float* quadratic);

    void RecomputeRates() const;

    float m_distance;
    mutable bool m_needRecompute;
    mutable float m_constant;
    mutable float m_linear;
    mutable float m_quadratic;
};

class PointLight : public LightSource
{
public:
    explicit PointLight(
        const Vector3f& position,
        const Vector3f& ambient,
        const Vector3f& diffuse,
        const Vector3f& specular,
        float m_distance);

    const Attenuation& GetAttenuation() const { return m_attenuation; }
    Attenuation& SetAttenuation() { return m_attenuation; }

    const Vector3f& GetPosition() const { return m_position; }

private:
    Vector3f m_position;
    Attenuation m_attenuation;
};

using PointLightPtr = std::shared_ptr<PointLight>;