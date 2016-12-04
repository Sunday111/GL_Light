#pragma once

#include "Render/Shaders/ShaderProgram.h"

#include <memory>

enum class PolygonMode
{
    Point,
    Line,
    Face
};

enum class FacetSide
{
    Front,
    Back,
    FrontAndBack
};

class IMaterial
{
public:
    virtual void PrepareContext() const = 0;

    virtual void SetPolygonMode(PolygonMode mode) = 0;
    virtual PolygonMode GetPolygonMode() const = 0;

    virtual void SetFacetSide(FacetSide facetSide) = 0;
    virtual FacetSide GetFacetSide() const = 0;

    virtual const ShaderProgramPtr& GetShaderProgram() const = 0;

    virtual ~IMaterial() {};
};

using IMaterialPtr = std::shared_ptr<IMaterial>;