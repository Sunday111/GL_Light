#include <set>
#include "Scene/Model3d.h"
#include "Render/VertexBufferObject.h"

namespace
{

class RenderCache
{
public:
    ElementBufferObjectPtr GetEBO_For(const MeshDataPtr& md)
    {
        const IndexBlobPtr& idxBlob = md->GetIndexData();

        for (auto& ebo : m_ebo)
        {
            if (ebo->GetIndexBlob() == idxBlob)
            {
                return ebo;
            }
        }

        VertexBufferObjectPtr vbo = GetVBO_For(md->GetVertexData());

        ElementBufferObjectPtr ebo = std::make_shared<ElementBufferObject>(
            vbo, idxBlob);

        m_ebo.insert(ebo);

        return ebo;
    }

    VertexBufferObjectPtr GetVBO_For(const VertexBlobPtr& vertBlob)
    {
        for (auto& vbo : m_vbo)
        {
            if (vbo->GetVertexBlob() == vertBlob)
            {
                return vbo;
            }
        }

        VertexBufferObjectPtr result = std::make_shared<VertexBufferObject>(
            std::make_shared<VertexArrayObject>(), vertBlob);

        m_vbo.insert(result);

        return result;
    }

private:
    std::set<ElementBufferObjectPtr> m_ebo;
    std::set<VertexBufferObjectPtr> m_vbo;
};

static RenderCache s_renderCache;

}

Model3d::Model3d(
    const MeshDataPtr& meshData,
    const IMaterialPtr& material) :
    m_flag(UpdateFlag::NormalMatrix),
    m_meshData(meshData),
    m_material(material)
{
    assert(m_meshData != nullptr && material != nullptr);
    m_elemBuffer = s_renderCache.GetEBO_For(m_meshData);
}

void Model3d::Draw() const
{
    m_material->GetShaderProgram()->Use();

    const GLuint modelLoc =
        m_material->GetShaderProgram()->TryGetUniformLocation("model");

    if (modelLoc != 0)
    {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(GetMatrix()));
    }

    const GLuint normalMatrixLoc =
        m_material->GetShaderProgram()->TryGetUniformLocation("normalMatrix");

    if (normalMatrixLoc != 0)
    {
        glUniformMatrix3fv(normalMatrixLoc,
            1, GL_FALSE, glm::value_ptr(GetNormalMatrix()));
    }

    m_material->PrepareContext();
    m_elemBuffer->Draw();
}

const glm::vec3 Model3d::GetPosition() const
{
    return glm::vec3(m_modelMatrix[3]);
}

void Model3d::SetMatrix(const glm::mat4& value)
{
    m_flag &= UpdateFlag::NormalMatrix;
    m_modelMatrix = value;
}

const glm::mat3& Model3d::GetNormalMatrix() const
{
    if ((m_flag & UpdateFlag::NormalMatrix) != UpdateFlag::Empty)
    {
        RecomputeNormalMatrix();
    }

    return m_normalMatrix;
}

void Model3d::RecomputeNormalMatrix() const
{
    m_flag &= (UpdateFlag::NormalMatrix);
    m_normalMatrix = glm::mat3(glm::transpose(glm::inverse(m_modelMatrix)));
}