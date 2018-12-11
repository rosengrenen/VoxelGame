#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera
{
public:
    /* Public methods */
    Camera(int x, int y, int z);

    glm::mat4 ViewMatrix();

    void MoveUp(double dt);
    void MoveDown(double dt);

    void MoveRight(double dt);
    void MoveLeft(double dt);

    void MoveForward(double dt);
    void MoveBackwards(double dt);

    void MouseMovement(int dx, int dy);

    // TODO: remove
    const double& yaw() const
    {
        return m_yaw;
    }

    const double& pitch() const
    {
        return m_pitch;
    }

    const glm::vec3& position() const
    {
        return m_position;
    }

    bool SphereInFrustum(const glm::vec3& center, float radius);

private:
    /* Private methods */
    void UpdateViewMatrix();
    glm::vec3 CalculateFrontVector(double yaw, double pitch) const;
    glm::vec3 CalculateRightVector(const glm::vec3& front, const glm::vec3& worldUp) const;
    glm::vec3 CalculateUpVector(const glm::vec3& right, const glm::vec3& front) const;

private:
    /* Private members */
    glm::mat4 m_viewMatrix = glm::mat4(1.0f);
    glm::vec3 m_position;
    glm::vec3 m_up;
    glm::vec3 m_front;
    glm::vec3 m_right;
    double m_yaw = 0.0;
    double m_pitch = 0.0;

    double m_changed = true;

    static constexpr double MOVEMENT_SPEED = 10;
    static constexpr double MOUSE_SENSITIVITY = 0.08;
};
