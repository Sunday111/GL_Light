#include "Scene/Materials/TexturedMaterial.h"

TexturedMaterial::TexturedMaterial(const ShaderProgramPtr& shader) :
    Material(shader),
    m_shininess(128.f)
{}

void TexturedMaterial::PrepareContext() const
{
    Material::PrepareContext();

    const ShaderProgramPtr& shader = GetShaderProgram();

    if (shader == nullptr)
    {
        return;
    }

    int uniformIndex = 0;

    if (m_ambient != nullptr)
    {
        glUniform1i(shader->GetUniformLocation("material.ambient"), uniformIndex);
        m_ambient->Use(uniformIndex++);
    }

    if (m_diffuse != nullptr)
    {
        glUniform1i(shader->GetUniformLocation("material.diffuse"), uniformIndex);
        m_diffuse->Use(uniformIndex++);
    }

    if (m_specular != nullptr)
    {
        glUniform1i(shader->GetUniformLocation("material.specular"), uniformIndex);
        m_specular->Use(uniformIndex++);
    }

    glUniform1f(shader->GetUniformLocation("material.shininess"), m_shininess);

}