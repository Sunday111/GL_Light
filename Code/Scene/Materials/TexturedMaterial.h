#pragma once

#include "Base/EnumFlags.h"
#include "Render/Texture.h"
#include "Render/Shaders/ShaderProgram.h"
#include "Scene/Materials/Material.h"

//Default material class
class TexturedMaterial : public Material
{
public:
    explicit TexturedMaterial(const ShaderProgramPtr& shader);

    void PrepareContext() const override;

    void SetAmbient(const Texture2dPtr& ambient) { m_ambient = ambient; }
    void SetDiffuse(const Texture2dPtr& diffuse) { m_diffuse = diffuse; }
    void SetSpecular(const Texture2dPtr& specular) { m_specular = specular; }

private:
    float m_shininess;
    Texture2dPtr m_ambient;
    Texture2dPtr m_diffuse;
    Texture2dPtr m_specular;
};

using TexturedMaterialPtr = std::shared_ptr<TexturedMaterial>;