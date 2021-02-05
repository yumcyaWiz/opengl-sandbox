#ifndef _OGLS_LIGHT_H
#define _OGLS_LIGHT_H

#include "glm/glm.hpp"

namespace ogls {

enum class LightType {
  Point,
  Directional,
};

class Light {
 public:
  glm::vec3 ke;
  LightType lightType;

  glm::vec3 position;  // for point light
  float radius;        // for point light

  glm::vec3 direction;  // for directional light

  Light(const glm::vec3& ke, const LightType& lightType)
      : ke(ke), lightType(lightType) {}
};

}  // namespace ogls

#endif