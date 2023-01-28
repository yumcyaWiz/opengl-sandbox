#pragma once

#include <cmath>

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace ogls
{

enum class CameraMovement {
  FORWARD,
  BACKWARD,
  RIGHT,
  LEFT,
  UP,
  DOWN,
};

class Camera
{
 public:
  glm::vec3 camPos;      // camera position
  glm::vec3 camForward;  // camera forward direction
  glm::vec3 camRight;    // camera right direction
  glm::vec3 camUp;       // camera up direction

  float fov;              // field of view
  float movementSpeed;    // camera movement speed
  float lookAroundSpeed;  // camera look around speed
  float phi;              // camera forward direction in spherical coordinates
  float theta;            // camera fowrward direction in spherical coordinates

  Camera();

  glm::mat4 computeViewMatrix() const;
  glm::mat4 computeProjectionMatrix(int width, int height) const;
  glm::mat4 computeViewProjectionMatrix(int width, int height) const;

  // reset camera parameters
  void reset();

  // move camera
  void move(const CameraMovement& direction, float ds);

  // look around camera forward
  void lookAround(float dPhi, float dTheta);
};

}  // namespace ogls