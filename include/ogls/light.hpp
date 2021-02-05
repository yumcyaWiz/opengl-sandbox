#ifndef _OGLS_LIGHT_H
#define _OGLS_LIGHT_H

#include "glm/glm.hpp"

namespace ogls {

class Light {
 public:
  glm::vec3 ke;

  Light() : ke{0.0f} {}
  Light(const glm::vec3& ke) : ke{ke} {}
};

class PointLight : public Light {
 public:
  glm::vec3 position;  // for point light
  float radius;        // for point light

  PointLight() : position{0.0f}, radius{0.0f} {}
  PointLight(const glm::vec3& ke, const glm::vec3& position, float radius)
      : Light{ke}, position{position}, radius{radius} {}
};

class DirectionalLight : public Light {
 public:
  glm::vec3 direction;  // direction

  DirectionalLight() : direction{0.0f} {}
  DirectionalLight(const glm::vec3& ke, const glm::vec3& direction)
      : Light{ke}, direction{direction} {}
};

}  // namespace ogls

#endif