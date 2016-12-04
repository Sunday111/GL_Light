#pragma once

#include "Scene/MeshData.h"
#include "Scene/Materials/IMaterial.h"

#include "Render/ElementBufferObject.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model3d
{
public:
    explicit Model3d(
        const MeshDataPtr& meshData,
        const IMaterialPtr& material);

    void Draw() const;

    const MeshDataPtr& GetMeshData() const { return m_meshData; }

    const IMaterialPtr& GetMaterial() const { return m_material; }

    const glm::vec3 GetPosition() const;

    const glm::mat4& GetMatrix() const
    {
        return m_modelMatrix;
    }

    void SetMatrix(const glm::mat4& value);

    const glm::mat3& GetNormalMatrix() const;

private:
    enum class UpdateFlag : uint8_t
    {
        Empty        = 0,
        NormalMatrix = 1,
    };

    void RecomputeNormalMatrix() const;

    ENUM_FLAG_OPERATORS_CLASS(UpdateFlag);

    mutable UpdateFlag m_flag;
    mutable glm::mat3 m_normalMatrix;

    glm::mat4 m_modelMatrix;
    MeshDataPtr m_meshData;
    IMaterialPtr m_material;
    ElementBufferObjectPtr m_elemBuffer;
};

using Model3dPtr = std::shared_ptr<Model3d>;