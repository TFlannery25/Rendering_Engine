#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 fwd, glm::vec3 upDir,
               float fieldOfView, float aspect, float nearP, float farP)
    : position(pos), forward(glm::normalize(fwd)), up(upDir),
      fov(fieldOfView), aspectRatio(aspect), nearPlane(nearP), farPlane(farP)
{
    // Derive initial yaw/pitch from the provided forward vector so
    // Rotate() stays consistent with whatever direction was passed in.
    forward = glm::normalize(fwd);
    pitch = glm::degrees(asin(forward.y));
    yaw   = glm::degrees(atan2(forward.z, forward.x));
    UpdateVectors();
}

void Camera::UpdateVectors()
{
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(dir);
    right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, forward));
}

void Camera::Rotate(float deltaYaw, float deltaPitch)
{
    yaw   += deltaYaw;
    pitch += deltaPitch;
    pitch  = glm::clamp(pitch, -89.0f, 89.0f); // prevent gimbal flip
    UpdateVectors();
}

void Camera::MoveForward(float amount)
{
    position += forward * amount;
}

void Camera::MoveRight(float amount)
{
    position += right * amount;
}

void Camera::MoveUp(float amount)
{
    position += glm::vec3(0.0f, 1.0f, 0.0f) * amount;
}
