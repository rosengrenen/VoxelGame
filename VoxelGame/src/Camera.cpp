#include "pch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(int x, int y, int z) : m_position(x, y, z)
{
    UpdateViewMatrix();
}

glm::mat4 Camera::ViewMatrix()
{
    UpdateViewMatrix();

    return m_viewMatrix;
}

void Camera::MoveUp(double dt)
{
    m_changed = true;
    m_position.y += MOVEMENT_SPEED * dt;
}

void Camera::MoveDown(double dt)
{
    m_changed = true;
    m_position.y -= MOVEMENT_SPEED * dt;
}

void Camera::MoveRight(double dt)
{
    m_changed = true;
    glm::vec2 direction = glm::normalize(glm::vec2(m_right.x, m_right.z));
    m_position.x += direction.x * MOVEMENT_SPEED * dt;
    m_position.z += direction.y * MOVEMENT_SPEED * dt;
}

void Camera::MoveLeft(double dt)
{
    m_changed = true;
    glm::vec2 direction = glm::normalize(glm::vec2(m_right.x, m_right.z));
    m_position.x -= direction.x * MOVEMENT_SPEED * dt;
    m_position.z -= direction.y * MOVEMENT_SPEED * dt;
}

void Camera::MoveForward(double dt)
{
    m_changed = true;
    glm::vec2 direction = glm::normalize(glm::vec2(m_front.x, m_front.z));
    m_position.x += direction.x * MOVEMENT_SPEED * dt;
    m_position.z += direction.y * MOVEMENT_SPEED * dt;
}

void Camera::MoveBackwards(double dt)
{
    m_changed = true;
    glm::vec2 direction = glm::normalize(glm::vec2(m_front.x, m_front.z));
    m_position.x -= direction.x * MOVEMENT_SPEED * dt;
    m_position.z -= direction.y * MOVEMENT_SPEED * dt;
}

void Camera::MouseMovement(int dx, int dy)
{
    m_changed = true;
    m_yaw += dx * MOUSE_SENSITIVITY;
    m_pitch -= dy * MOUSE_SENSITIVITY;

    m_yaw = std::fmod(m_yaw, 360.0);

    if (m_pitch > 89.0)
    {
        m_pitch = 89.0;
    }
    else if (m_pitch < -89.0)
    {
        m_pitch = -89.0;
    }
}

//bool Camera::SphereInFrustum(const glm::vec3& center, float radius)
//{
//    float distance;
//    int result = INSIDE;
//
//    for (int i = 0; i < 6; i++) {
//        distance = pl[i].distance(p);
//        if (distance < -radius)
//            return OUTSIDE;
//        else if (distance < radius)
//            result = INTERSECT;
//    }
//    return(result);
//}

void Camera::UpdateViewMatrix()
{
    if (!m_changed)
    {
        return;
    }
    m_changed = false;

    static const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    m_front = CalculateFrontVector(m_pitch, m_yaw);
    m_right = CalculateRightVector(m_front, worldUp);
    m_up = CalculateUpVector(m_right, m_front);

    m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::vec3 Camera::CalculateFrontVector(double pitch, double yaw) const
{
    glm::vec3 front(
        glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw)),
        glm::sin(glm::radians(pitch)),
        glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw))
    );
    return glm::normalize(front);
}

glm::vec3 Camera::CalculateRightVector(const glm::vec3& front, const glm::vec3& worldUp) const
{
    return glm::normalize(glm::cross(front, worldUp));
}

glm::vec3 Camera::CalculateUpVector(const glm::vec3& right, const glm::vec3& front) const
{
    return glm::normalize(glm::cross(right, front));
}
