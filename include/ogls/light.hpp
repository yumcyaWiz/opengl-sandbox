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

  Light(const glm::vec3& ke, const LightType& lightType)
      : ke(ke), lightType(lightType) {}
};

}  // namespace ogls

#endif