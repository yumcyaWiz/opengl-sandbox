#include "ogls/camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace ogls {

Camera::Camera()
    : camPos{0.0f},
      camForward{0.0, 0.0, -1.0f},
      camRight{1.0f, 0.0f, 0.0f},
      camUp{0.0f, 1.0f, 0.0f},
      fov(45.0f),
      movementSpeed(1.0f),
      lookAroundSpeed(0.5f),
      phi(270.0f),
      theta(90.0f) {}

glm::mat4 Camera::computeViewMatrix() const {
  return glm::lookAt(camPos, camPos + camForward, camUp);
}

glm::mat4 Camera::computeProjectionMatrix(int width, int height) const {
  return glm::perspective(glm::radians(fov), static_cast<float>(width) / height,
                          0.1f, 10000.0f);
}

void Camera::reset() { Camera(); }

void Camera::move(const CameraMovement& direction, float ds) {
  const float velocity = movementSpeed * ds;
  switch (direction) {
    case CameraMovement::FORWARD:
      camPos += velocity * camForward;
      break;
    case CameraMovement::BACKWARD:
      camPos -= velocity * camForward;
      break;
    case CameraMovement::RIGHT:
      camPos += velocity * camRight;
      break;
    case CameraMovement::LEFT:
      camPos -= velocity * camRight;
      break;
    case CameraMovement::UP:
      camPos += velocity * camUp;
      break;
    case CameraMovement::DOWN:
      camPos -= velocity * camUp;
      break;
  }
}

void Camera::lookAround(float dPhi, float dTheta) {
  // update phi, theta
  phi += lookAroundSpeed * dPhi;
  if (phi < 0.0f) phi = 360.0f;
  if (phi > 360.0f) phi = 0.0f;

  theta += lookAroundSpeed * dTheta;
  if (theta < 0.0f) theta = 180.0f;
  if (theta > 180.0f) theta = 0.0f;

  // set camera directions
  const float phiRad = glm::radians(phi);
  const float thetaRad = glm::radians(theta);
  camForward =
      glm::vec3(std::cos(phiRad) * std::sin(thetaRad), std::cos(thetaRad),
                std::sin(phiRad) * std::sin(thetaRad));
  camRight = glm::normalize(glm::cross(camForward, glm::vec3(0, 1.0f, 0)));
  camUp = glm::normalize(glm::cross(camRight, camForward));
}

}  // namespace ogls