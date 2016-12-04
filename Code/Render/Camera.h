#pragma once

#include "Base/EnumFlags.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:

    Camera(float aspect, float near, float far, float fov = 45.f);

    float GetFOV() const { return m_fov; }
    void SetFOV(float value);

    float GetAspect() const { return m_aspect; }
    void SetAspect(float value);

    float GetNear() const { return m_near; }
    void SetNear(float value);

    float GetFar() const { return m_far; }
    void SetFar(float value);

    /* Reverse camera direction */
    const glm::vec3& GetFront() const { return m_front; }
    void SetFront(const glm::vec3& value);

    const glm::vec3& GetPosition() const { return m_position; }
    void SetPosition(const glm::vec3& value);

    const glm::vec3& GetUp() const { return m_up; }
    void SetUp(const glm::vec3& value);

    /* This is not editable value */
    const glm::vec3& GetRight() const;

    /* This is not editable value */
    const glm::mat4& GetViewMatrix() const;

    /* This is not editable value */
    const glm::mat4& GetProjection() const;

    void SetAngles(float yaw, float pitch);

    float GetYaw() const
    {
        return glm::yaw(glm::quat(GetViewMatrix()));
    }

    float GetPitch() const
    {
        return glm::pitch(glm::quat(GetViewMatrix()));
    }

    float GetRoll() const
    {
        return glm::roll(glm::quat(GetViewMatrix()));
    }

protected:
    void RecomputeRightVector() const;
    void RecomputeViewMatrix() const;
    void RecomputeProjectionMatrix() const;

private:
    enum class Flag : uint32_t
    {
        Empty                = 0,
        NeedUpdateView       = 1,
        NeedUpdateRight      = 2,
        NeedUpdateProjection = 4
    };

    ENUM_FLAG_OPERATORS_CLASS(Flag);

    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;
    glm::vec3 m_front;
    glm::vec3 m_position;
    glm::vec3 m_up;

    mutable Flag m_flags;
    mutable glm::vec3 m_right;
    mutable glm::mat4 m_view;
    mutable glm::mat4 m_projection;
};