#include "camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace ogls
{

Camera::Camera()
    : cam_pos{0.0f},
      cam_forward{0.0, 0.0, -1.0f},
      cam_right{1.0f, 0.0f, 0.0f},
      cam_up{0.0f, 1.0f, 0.0f},
      fov(45.0f),
      movement_speed(1.0f),
      look_around_speed(0.5f),
      phi(270.0f),
      theta(90.0f)
{
}

glm::mat4 Camera::computeViewMatrix() const
{
    return glm::lookAt(cam_pos, cam_pos + cam_forward, cam_up);
}

glm::mat4 Camera::computeProjectionMatrix(int width, int height) const
{
    return glm::perspective(glm::radians(fov),
                            static_cast<float>(width) / height, 0.1f, 10000.0f);
}

glm::mat4 Camera::computeViewProjectionMatrix(int width, int height) const
{
    return computeProjectionMatrix(width, height) * computeViewMatrix();
}

void Camera::reset() { *this = Camera(); }

void Camera::move(const CameraMovement& direction, float ds)
{
    const float velocity = movement_speed * ds;
    switch (direction) {
        case CameraMovement::FORWARD:
            cam_pos += velocity * cam_forward;
            break;
        case CameraMovement::BACKWARD:
            cam_pos -= velocity * cam_forward;
            break;
        case CameraMovement::RIGHT:
            cam_pos += velocity * cam_right;
            break;
        case CameraMovement::LEFT:
            cam_pos -= velocity * cam_right;
            break;
        case CameraMovement::UP:
            cam_pos += velocity * cam_up;
            break;
        case CameraMovement::DOWN:
            cam_pos -= velocity * cam_up;
            break;
    }
}

void Camera::lookAround(float dPhi, float dTheta)
{
    // update phi, theta
    phi += look_around_speed * dPhi;
    if (phi < 0.0f) phi = 360.0f;
    if (phi > 360.0f) phi = 0.0f;

    theta += look_around_speed * dTheta;
    if (theta < 0.0f) theta = 180.0f;
    if (theta > 180.0f) theta = 0.0f;

    // set camera directions
    const float phiRad = glm::radians(phi);
    const float thetaRad = glm::radians(theta);
    cam_forward =
        glm::vec3(std::cos(phiRad) * std::sin(thetaRad), std::cos(thetaRad),
                  std::sin(phiRad) * std::sin(thetaRad));
    cam_right = glm::normalize(glm::cross(cam_forward, glm::vec3(0, 1.0f, 0)));
    cam_up = glm::normalize(glm::cross(cam_right, cam_forward));
}

}  // namespace ogls