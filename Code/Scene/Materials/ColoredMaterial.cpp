#include "Scene/Materials/ColoredMaterial.h"

ColoredMaterial::ColoredMaterial(const ShaderProgramPtr& shader) :
    Material(shader)
{}

void ColoredMaterial::PrepareContext() const
{
    Material::PrepareContext();

    const ShaderProgramPtr& shader = GetShaderProgram();

    if (shader == nullptr)
    {
        return;
    }

    glUniform3f(shader->GetUniformLocation("inputColor"),
        m_color.x(), m_color.y(), m_color.z());

    glPolygonMode(GetGlFacetSide(), GetGlPolygonMode());
}