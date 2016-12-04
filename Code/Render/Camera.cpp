#include "Render/Camera.h"

Camera::Camera(float aspect, float near, float far, float fov) :
    m_fov(fov), m_aspect(aspect),
    m_near(near), m_far(far),
    m_front(glm::vec3(0.f, 0.f, -1.f)),
    m_position(glm::vec3(0.f, 0.f, 0.f)),
    m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_flags(Flag::NeedUpdateRight | Flag::NeedUpdateView | Flag::NeedUpdateProjection)
{
}

void Camera::SetFOV(float value)
{
    m_flags |= Flag::NeedUpdateProjection;
    m_fov = value;
}

void Camera::SetAspect(float value)
{
    m_flags |= Flag::NeedUpdateProjection;
    m_aspect = value;
}

void Camera::SetNear(float value)
{
    m_flags |= Flag::NeedUpdateProjection;
    m_near = value;
}

void Camera::SetFar(float value)
{
    m_flags |= Flag::NeedUpdateProjection;
    m_far = value;
}

void Camera::SetFront(const glm::vec3& value)
{
    m_flags |= Flag::NeedUpdateRight | Flag::NeedUpdateView;
    m_front = value;
}

void Camera::SetPosition(const glm::vec3& value)
{
    m_flags |= Flag::NeedUpdateView;
    m_position = value;
}

void Camera::SetUp(const glm::vec3& value)
{
    m_flags |= Flag::NeedUpdateRight | Flag::NeedUpdateView;
    m_up = value;
}

const glm::vec3& Camera::GetRight() const
{
    if ((m_flags & Flag::NeedUpdateRight) != Flag::Empty)
    {
        RecomputeRightVector();
    }

    return m_right;
}

const glm::mat4& Camera::GetViewMatrix() const
{
    if ((m_flags & Flag::NeedUpdateView) != Flag::Empty)
    {
        RecomputeViewMatrix();
    }

    return m_view;
}

/* This is not editable value */
const glm::mat4& Camera::GetProjection() const
{
    if ((m_flags & Flag::NeedUpdateProjection) != Flag::Empty)
    {
        RecomputeProjectionMatrix();
    }

    return m_projection;
}

void Camera::SetAngles(float yaw, float pitch)
{
    glm::vec3 front;
    front.x = cos(yaw) * cos(pitch);
    front.y = sin(pitch);
    front.z = sin(yaw) * cos(pitch);
    SetFront(front);
}

void Camera::RecomputeRightVector() const
{
    m_right = glm::normalize(glm::cross(m_front, m_up));
    m_flags &= (~Flag::NeedUpdateRight);
}

void Camera::RecomputeViewMatrix() const
{
    m_view = glm::lookAt(m_position, m_position + m_front, m_up);
    m_flags &= (~Flag::NeedUpdateView);
}

void Camera::RecomputeProjectionMatrix() const
{
    m_projection = glm::perspective(m_fov, m_aspect, m_near, m_far);
    m_flags &= (~Flag::NeedUpdateProjection);
}