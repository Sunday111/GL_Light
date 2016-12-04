#pragma once

#include "Base/EnumFlags.h"
#include "Base/Geom/Vector.h"
#include "Render/Shaders/ShaderProgram.h"
#include "Scene/Materials/Material.h"

class ColoredMaterial : public Material
{
public:
    explicit ColoredMaterial(const ShaderProgramPtr& shader);

    void PrepareContext() const override;

    void SetColor(const Vector3f& color) { m_color = color; }
    const Vector3f& GetColor() const { return m_color; }

private:
    Vector3f m_color;
};

using ColoredMaterialPtr = std::shared_ptr<ColoredMaterial>;