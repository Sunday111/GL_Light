#include "Scene/Lights/LightsArray.h"

namespace
{
    static void PrepareColorContext(std::string& prefix,
        const ShaderProgramPtr& shader,
        const LightSource* light)
    {
        assert(light != nullptr && shader != nullptr);

        const size_t prevSize = prefix.size();

        prefix += "color.";
        const size_t colorConstSize = prefix.size();

        prefix.resize(colorConstSize);
        prefix += "ambient";
        glUniform3f(
            shader->GetUniformLocation(prefix.c_str()),
            light->GetAmbient().x(),
            light->GetAmbient().y(),
            light->GetAmbient().z());

        prefix.resize(colorConstSize);
        prefix += "diffuse";
        glUniform3f(
            shader->GetUniformLocation(prefix.c_str()),
            light->GetDiffuse().x(),
            light->GetDiffuse().y(),
            light->GetDiffuse().z());

        prefix.resize(colorConstSize);
        prefix += "specular";
        glUniform3f(
            shader->GetUniformLocation(prefix.c_str()),
            light->GetSpecular().x(),
            light->GetSpecular().y(),
            light->GetSpecular().z());

        prefix.resize(prevSize);
    }

    static void PrepareAttenuationContext(std::string prefix,
        const ShaderProgramPtr& shader,
        const Attenuation& attenuation)
    {
        assert(shader != nullptr);

        const size_t prevSize = prefix.size();

        prefix += "attenuation.";

        const size_t constSize = prefix.size();

        prefix.resize(constSize);
        prefix += "constant";
        glUniform1f(
            shader->GetUniformLocation(prefix.c_str()),
            attenuation.GetConstantRate());

        prefix.resize(constSize);
        prefix += "linear";
        glUniform1f(
            shader->GetUniformLocation(prefix.c_str()),
            attenuation.GetLinearRate());

        prefix.resize(constSize);
        prefix += "quadratic";
        glUniform1f(
            shader->GetUniformLocation(prefix.c_str()),
            attenuation.GetQuadraticRate());

        prefix.resize(prevSize);
    }
}

LightsArray::LightsArray(
    std::vector<PointLightPtr> pointLights,
    std::vector<DirectionalLightPtr> directionalLights,
    std::vector<SpotLightPtr> spotLights,
    const char* VS, const char* FS,
    std::ostream* logstream) :
    m_pointLights(std::move(pointLights)),
    m_directionalLights(std::move(directionalLights)),
    m_spotLights(std::move(spotLights))
{
    m_pointLightsCapacity =
        m_pointLights.size() > 4 ?
        m_pointLights.size() : 4;

    m_directionalLightsCapacity =
        m_directionalLights.size() > 4 ?
        m_directionalLights.size() : 4;

    m_spotLightsCapacity =
        m_spotLights.size() > 4 ?
        m_spotLights.size() : 4;

    std::string additionalCode =
        "#version 330 core\r\n"
        "#define VERSION_DEFINED\r\n"
        "#define POINT_LIGHTS\r\n"
        "#define DIRECTIONAL_LIGHTS\r\n"
        "#define SPOT_LIGHTS\r\n"
        "#define POINT_LIGHTS_CAPACITY ";

    additionalCode += std::to_string(m_pointLightsCapacity);
    additionalCode += "\r\n";

    additionalCode += "#define DIRECTIONAL_LIGHTS_CAPACITY ";
    additionalCode += std::to_string(m_directionalLightsCapacity);
    additionalCode += "\r\n";

    additionalCode += "#define SPOT_LIGHTS_CAPACITY ";
    additionalCode += std::to_string(m_spotLightsCapacity);
    additionalCode += "\r\n";

    VertexShaderPtr vertexShader = std::make_shared<VertexShader>(VS, nullptr, logstream);
    FragmentShaderPtr texturedMatFS = std::make_shared<FragmentShader>(FS, additionalCode.c_str(), logstream);
    m_shader = std::make_shared<ShaderProgram>(vertexShader, texturedMatFS, logstream);
}

void LightsArray::PrepareContext()
{
    m_shader->Use();

    glUniform1i(m_shader->GetUniformLocation("pointLightsCount"),
        static_cast<int>(m_pointLights.size()));

    for (size_t i = 0; i < m_pointLights.size(); ++i)
    {
        const auto& pointLight = m_pointLights[i];

        m_cachedUniformName = "pointLights[";
        m_cachedUniformName += std::to_string(i);
        m_cachedUniformName += "].";

        const size_t constSize = m_cachedUniformName.size();

        m_cachedUniformName += "position";
        glUniform3f(
            m_shader->GetUniformLocation(m_cachedUniformName.c_str()),
            pointLight->GetPosition().x(),
            pointLight->GetPosition().y(),
            pointLight->GetPosition().z());

        m_cachedUniformName.resize(constSize);
        PrepareAttenuationContext(m_cachedUniformName, m_shader, pointLight->GetAttenuation());
        PrepareColorContext(m_cachedUniformName, m_shader, pointLight.get());
    }

    glUniform1i(m_shader->GetUniformLocation("directionalLightsCount"),
        static_cast<int>(m_directionalLights.size()));

    for (size_t i = 0; i < m_directionalLights.size(); ++i)
    {
        const auto& directionalLight = m_directionalLights[i];

        m_cachedUniformName = "directionalLights[";
        m_cachedUniformName += std::to_string(i);
        m_cachedUniformName += "].";

        const size_t constSize = m_cachedUniformName.size();

        m_cachedUniformName += "direction";
        glUniform3f(
            m_shader->GetUniformLocation(m_cachedUniformName.c_str()),
            directionalLight->GetDirection().x(),
            directionalLight->GetDirection().y(),
            directionalLight->GetDirection().z());

        m_cachedUniformName.resize(constSize);

        PrepareColorContext(m_cachedUniformName, m_shader, directionalLight.get());
    }

    glUniform1i(m_shader->GetUniformLocation("spotLightsCount"),
        static_cast<int>(m_spotLights.size()));

    for (size_t i = 0; i < m_spotLights.size(); ++i)
    {
        const auto& spotLight = m_spotLights[i];

        m_cachedUniformName = "spotLights[";
        m_cachedUniformName += std::to_string(i);
        m_cachedUniformName += "].";

        const size_t constSize = m_cachedUniformName.size();

        m_cachedUniformName.resize(constSize);
        m_cachedUniformName += "cosInnerAngle";
        glUniform1f(
            m_shader->GetUniformLocation(m_cachedUniformName.c_str()),
            spotLight->GetInnerAngleCos());

        m_cachedUniformName.resize(constSize);
        m_cachedUniformName += "cosOuterAngle";
        glUniform1f(
            m_shader->GetUniformLocation(m_cachedUniformName.c_str()),
            spotLight->GetOuterAngleCos());

        m_cachedUniformName.resize(constSize);
        m_cachedUniformName += "direction";
        glUniform3f(
            m_shader->GetUniformLocation(m_cachedUniformName.c_str()),
            spotLight->GetDirection().x(),
            spotLight->GetDirection().y(),
            spotLight->GetDirection().z());

        m_cachedUniformName.resize(constSize);
        m_cachedUniformName += "position";
        glUniform3f(
            m_shader->GetUniformLocation(m_cachedUniformName.c_str()),
            spotLight->GetPosition().x(),
            spotLight->GetPosition().y(),
            spotLight->GetPosition().z());

        m_cachedUniformName.resize(constSize);

        PrepareColorContext(m_cachedUniformName, m_shader, spotLight.get());
    }

    m_cachedUniformName.clear();
}

void LightsArray::AddSpotLight(const SpotLightPtr& spotlight)
{
    if (m_spotLights.size() >= m_spotLightsCapacity)
    {
        assert(!"Not implemented yet");
    }

    m_spotLights.push_back(spotlight);
}

void LightsArray::ClearSpotLights()
{
    m_spotLights.clear();
}