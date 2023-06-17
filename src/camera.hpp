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
  glm::vec3 cam_pos;      // camera position
  glm::vec3 cam_forward;  // camera forward direction
  glm::vec3 cam_right;    // camera right direction
  glm::vec3 cam_up;       // camera up direction

  float fov;                // field of view
  float movement_speed;     // camera movement speed
  float look_around_speed;  // camera look around speed
  float phi;                // camera forward direction in spherical coordinates
  float theta;  // camera fowrward direction in spherical coordinates

  Camera();

  glm::mat4 compute_view_matrix() const;
  glm::mat4 compute_projection_matrix(int width, int height) const;
  glm::mat4 compute_view_projection_matrix(int width, int height) const;

  // reset camera parameters
  void reset();

  // move camera
  void move(const CameraMovement& direction, float ds);

  // look around camera forward
  void look_around(float dPhi, float dTheta);
};

}  // namespace ogls