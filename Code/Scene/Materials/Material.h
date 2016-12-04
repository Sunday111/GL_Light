#pragma once

#include "Base/EnumFlags.h"
#include "Render/Shaders/ShaderProgram.h"
#include "Scene/Materials/IMaterial.h"

// GLEW
#define GLEW_STATIC
#include <GLEW/glew.h>

class Material : public IMaterial
{
public:

    void PrepareContext() const override;

    void SetFacetSide(FacetSide facetSide) override;
    FacetSide GetFacetSide() const override { return m_facetSide; }

    void SetPolygonMode(PolygonMode mode) override;
    PolygonMode GetPolygonMode() const override { return m_polygonMode; }

    virtual const ShaderProgramPtr& GetShaderProgram() const override
    {
        return m_shader;
    }

protected:
    explicit Material(const ShaderProgramPtr& shader);

    GLuint GetGlFacetSide() const;
    GLuint GetGlPolygonMode() const;

private:
    enum class UpdateFlag
    {
        Empty = 0,
        PolygonMode = 1,
        FacetSide = 2
    };

    ENUM_FLAG_OPERATORS_CLASS(UpdateFlag);

    mutable UpdateFlag m_flag;
    mutable GLuint m_glFacetSide;
    mutable GLuint m_glPolygonMode;

    float m_pointSize;
    float m_lineWidth;

    FacetSide m_facetSide;
    PolygonMode m_polygonMode;

    ShaderProgramPtr m_shader;
};