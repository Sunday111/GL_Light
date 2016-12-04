#pragma once

#include <vector>
#include "Scene/Lights/PointLight.h"
#include "Scene/Lights/DirectionalLight.h"
#include "Scene/Lights/SpotLight.h"
#include "Render/Shaders/ShaderProgram.h"

class LightsArray
{
public:
    explicit LightsArray(
        std::vector<PointLightPtr> pointLights,
        std::vector<DirectionalLightPtr> directionalLights,
        std::vector<SpotLightPtr> spotLights,
        const char* VS, const char* FS,
        std::ostream* ostream = nullptr);

    const ShaderProgramPtr& GetShader() const { return m_shader; }

    void PrepareContext();

    const std::vector<PointLightPtr>& GetPointLights() const { return m_pointLights; }
    const std::vector<SpotLightPtr>& GetSpotLights() const { return m_spotLights; }
    const std::vector<DirectionalLightPtr>& GetDirectionalLights() const { return m_directionalLights; }

    void AddSpotLight(const SpotLightPtr& spotlight);
    void ClearSpotLights();

private:

    size_t m_pointLightsCapacity;
    std::vector<PointLightPtr> m_pointLights;

    size_t m_directionalLightsCapacity;
    std::vector<DirectionalLightPtr> m_directionalLights;

    size_t m_spotLightsCapacity;
    std::vector<SpotLightPtr> m_spotLights;

    ShaderProgramPtr m_shader;

    mutable std::string m_cachedUniformName;
};

using LightsArrayPtr = std::shared_ptr<LightsArray>;