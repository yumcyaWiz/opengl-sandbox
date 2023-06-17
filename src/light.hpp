#pragma once

#include "glm/glm.hpp"

namespace ogls
{

class Light
{
 private:
  glm::vec3 ke;

 public:
  Light() : ke{0.0f} {}
  Light(const glm::vec3& ke) : ke{ke} {}

  glm::vec3 getKe() const { return ke; }
};

class PointLight : public Light
{
 private:
  glm::vec3 position;  // for point light
  float radius;        // for point light

 public:
  PointLight() : position{0.0f}, radius{0.0f} {}
  PointLight(const glm::vec3& ke, const glm::vec3& position, float radius)
      : Light{ke}, position{position}, radius{radius}
  {
  }

  glm::vec3 getPosition() const { return position; }
  float getRadius() const { return radius; }
};

class DirectionalLight : public Light
{
 private:
  glm::vec3 direction;  // direction

 public:
  DirectionalLight() : direction{0.0f} {}
  DirectionalLight(const glm::vec3& ke, const glm::vec3& direction)
      : Light{ke}, direction{direction}
  {
  }

  glm::vec3 getDirection() const { return direction; }
};

}  // namespace ogls