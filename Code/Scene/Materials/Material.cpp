#include <cassert>

#include "Scene/Materials/Material.h"

namespace
{
    static GLenum PolygonModeToGlenum(PolygonMode mode)
    {
        switch (mode)
        {
        case PolygonMode::Point: return GL_POINT;
        case PolygonMode::Line:  return GL_LINE;
        case PolygonMode::Face:  return GL_FILL;
        default:
            assert(!"Invalid mapping here");
            return 0;
        }
    }

    static GLenum FacetSideToGlenum(FacetSide face)
    {
        switch (face)
        {
        case FacetSide::Front:        return GL_FRONT;
        case FacetSide::Back:         return GL_BACK;
        case FacetSide::FrontAndBack: return GL_FRONT_AND_BACK;
        default:
            assert(!"Invalid mapping here");
            return 0;
        }
    }
}

Material::Material(const ShaderProgramPtr& shader) :
    m_flag(UpdateFlag::FacetSide | UpdateFlag::PolygonMode),
    m_facetSide(FacetSide::FrontAndBack),
    m_polygonMode(PolygonMode::Face),
    m_pointSize(2.f),
    m_lineWidth(2.f),
    m_shader(shader)
{}

void Material::PrepareContext() const
{
    if (m_shader == nullptr)
    {
        return;
    }

    m_shader->Use();

    glPolygonMode(GetGlFacetSide(), GetGlPolygonMode());

    switch (m_polygonMode)
    {
    case PolygonMode::Point:
        glPointSize(m_pointSize);
        break;
    case PolygonMode::Line:
        glLineWidth(m_lineWidth);
        break;
    }
}

void Material::SetFacetSide(FacetSide facetSide)
{
    if (m_facetSide != facetSide)
    {
        m_facetSide = facetSide;
        m_flag |= UpdateFlag::FacetSide;
    }
}

void Material::SetPolygonMode(PolygonMode mode)
{
    if (m_polygonMode != mode)
    {
        m_polygonMode = mode;
        m_flag |= UpdateFlag::PolygonMode;
    }
}

GLuint Material::GetGlFacetSide() const
{
    if ((m_flag & UpdateFlag::FacetSide) != UpdateFlag::Empty)
    {
        m_glFacetSide = FacetSideToGlenum(m_facetSide);
        m_flag &= (~UpdateFlag::FacetSide);
    }

    return m_glFacetSide;
}

GLuint Material::GetGlPolygonMode() const
{
    if ((m_flag & UpdateFlag::PolygonMode) != UpdateFlag::Empty)
    {
        m_glPolygonMode = PolygonModeToGlenum(m_polygonMode);
        m_flag &= (~UpdateFlag::PolygonMode);
    }

    return m_glPolygonMode;
}