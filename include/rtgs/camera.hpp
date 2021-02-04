#ifndef _RTGS_CAMERA_H
#define _RTGS_CAMERA_H
#include <cmath>

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace rtgs {

enum class CameraMovement {
  FORWARD,
  BACKWARD,
  RIGHT,
  LEFT,
  UP,
  DOWN,
};

class Camera {
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

  // initialize camera position, directions
  void reset();

  // move camera
  void move(const CameraMovement& direction, float deltaTime);

  // look around camera forward
  void lookAround(float dPhi, float dTheta);
};

}  // namespace rtgs

#endif